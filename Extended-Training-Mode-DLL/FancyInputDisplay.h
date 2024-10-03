#pragma once

template <typename T, int size>
class CircularBuffer {
public:

	CircularBuffer() {}

	T get(int i) {
		while (i < 0) {
			i += size;
		}
		i += index;
		return data[i % size];
	}

	void pushHead(const T& v) {
		index++;
		if (index >= size) {
			index = 0;
		}
		data[index] = v;
	}

	void pushTail(const T& v) {
		index--;
		if (index < 0) {
			index = size - 1;
		}
		data[index] = v;
	}

	T front() {
		return data[index];
	}

	T data[size];
	int index = 0; // index will be the head, index-1 will be the tail
};

typedef struct InputData {
	DWORD rawInput = 0;
	BYTE direction = ' ';
	BYTE buttons = 0;
	BYTE buttonString[5] = { ' ', ' ', ' ', ' ', '\0' };
	int length = 0;
} InputData;

class InputColumn {
public:

	InputColumn(unsigned addr_, float xVal_, int inputMaxLen_) {
		addr = addr_;
		xVal = xVal_;
		inputMaxLen = inputMaxLen_;

		inputs = std::vector<InputData>(60);
	}

	void update() {
		DWORD inputVal = *(DWORD*)(addr);

		BYTE buttons = (inputVal & 0xF00000) >> 20;
		BYTE direction = 0x80 + (inputVal & 0xF);

		rawInputIndex = (rawInputIndex + 1) % 60;
		rawInputData[rawInputIndex] = inputVal;
		
		//if (inputs[inputIndex].rawInput == inputVal) {
		if (inputs[inputIndex].buttons == buttons && inputs[inputIndex].direction == direction) {
			inputs[inputIndex].length++;
			if (inputs[inputIndex].length > 999) {
				inputs[inputIndex].length = 999;
			}
			return;
		}

		inputIndex--;
		if (inputIndex < 0) {
			inputIndex = inputMaxLen - 1;
		}

		inputs[inputIndex].direction = direction;
		inputs[inputIndex].rawInput = inputVal;
		inputs[inputIndex].buttons = buttons;

		BYTE* buf = inputs[inputIndex].buttonString;
		if (buttons & 0x1) {
			*buf = BUTTON_A;
			buf++;
		}

		if (buttons & 0x2) {
			*buf = BUTTON_B;
			buf++;
		}

		if (buttons & 0x4) {
			*buf = BUTTON_C;
			buf++;
		}

		if (buttons & 0x8) {
			*buf = BUTTON_D;
			buf++;
		}

		*buf = '\0';

		inputs[inputIndex].length = 1;
	}

	void draw() {

		float x;

		if (wWidth < (wHeight * 4.0f / 3.0f)) {
			x = xVal;
		} else {
			if (xVal < 300.0f) {
				x = -((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

				x *= (wWidth / (wHeight * 4.0f / 3.0f));
				x *= 640.0f;

				x += xVal;
			} else {
				x = ((wWidth - (wHeight * 4.0f / 3.0f)) / 2.0f) / wWidth;

				x *= (wWidth / (wHeight * 4.0f / 3.0f));
				x *= 640.0f;

				x = 640.0 + x - 100.0f;
			}
		}
	
		

		float y = 0.2f * 480.0f;

		float yVal = 124;
		for (int i = 0; i < inputMaxLen; i++) {

			int index = (i + inputIndex) % inputMaxLen;

			TextDrawSimple(x, yVal, 13, 0xFFFFFFFF, "%c%s", inputs[index].direction, inputs[index].buttonString);
			TextDraw(x + 55, yVal, 13, 0xFFFFFFFF, "%3d", inputs[index].length);

			RectDraw(x, yVal, 90, 12, 0x40000000);
			yVal += 13;
		}

	}

	void reset() {
		inputIndex = 0;
		for (int i = 0; i < inputMaxLen; i++) {
			inputs[i] = InputData();
		}

		rawInputIndex = 0;
		for (int i = 0; i < 60; i++) {
			rawInputData[i] = 0;
		}
	}

	unsigned addr;
	float xVal;
	int inputMaxLen;
	//InputData inputs[inputMaxLen];
	std::vector<InputData> inputs;
	int inputIndex = 0;
	
	DWORD rawInputData[60]; // store the last second of inputs for the input display. should it be more than this? maybe? 
	int rawInputIndex = 0;
};

InputColumn P1InputBar(0x00555134 + 0x02E7, 10.0f , 20);
InputColumn P2InputBar(0x00555C30 + 0x02E7, 545.0f, 20);

class InputDisplay {
public:

	typedef struct JoyLog {
		int dir;
		unsigned frame;
	} JoyLog;

	InputDisplay(float xPos_, float yPos_, float scale_, float cornerScale_, InputColumn* inputColumn_, int player_) : xPos(xPos_), yPos(yPos_), scale(scale_), cornerScale(cornerScale_), inputColumn(inputColumn_), player(player_) { }

	void handleDrag() {
		// this should really be done by giving information/a func callback to a global menu manager. but will work for now
		if (hasDrag && hasDrag != player) {
			return;
		}

		static KeyState lButton(VK_LBUTTON);

		if (mousePos.x > xPos - cornerScale * 1.1f &&
			mousePos.y > yPos - cornerScale * 1.1f &&
			mousePos.x < xPos + cornerScale * 3.15f &&
			mousePos.y < yPos + cornerScale * 1.1f
			) {
			if (lButton.keyHeld()) {
				xPos = mousePos.x;
				yPos = mousePos.y;
				hasDrag = player;
				return;
			}
		}

		hasDrag = 0;

	}

	void drawPoint(float x, float y, DWORD col = 0xFFD0D0D0, float size = 4.0f) {

		x -= (size / 2.0f);
		y -= (size / 2.0f);

		const D3DXVECTOR2 lineTopLeft(9.0f / 16.0f, 7.0f / 16.0);
		const D3DXVECTOR2 lineWidth(1.0f / 16.0f, 0.0f / 16.0f);
		const D3DXVECTOR2 lineHeight(0.0f / 16.0f, 1.0f / 16.0f);

		const Quad<MeltyVert> test(
			MeltyVert(x + 0, y + 0, lineTopLeft),
			MeltyVert(x + size, y + 0, lineTopLeft + lineWidth),
			MeltyVert(x + 0, y + size, lineTopLeft + lineHeight),
			MeltyVert(x + size, y + size, lineTopLeft + lineWidth + lineHeight),
			col
		);

		meltyVertData.add(test);
	}

	void numpadToCoords(int v, D3DXVECTOR2& out) {

		out.x = xPos;
		out.y = yPos;

		switch (v) {
		default:
		case 0:
		case 5:
			break;
		case 1:
			out.x -= scale;
			out.y += scale;
			break;
		case 2:
			out.y += cornerScale;
			break;
		case 3:
			out.x += scale;
			out.y += scale;
			break;
		case 4:
			out.x -= cornerScale;
			break;
		case 6:
			out.x += cornerScale;
			break;
		case 7:
			out.x -= scale;
			out.y -= scale;
			break;
		case 8:
			out.y -= cornerScale;
			break;
		case 9:
			out.x += scale;
			out.y -= scale;
			break;
		}

	}

	void drawLine(int a, int b, DWORD colA, DWORD colB, float lineBaseWidth = 6.0f, float size = 8.0f) {
		// a and b are numpad directions. 
		// good luck

		if (a == b) {
			return;
		}

		D3DXVECTOR2 start;
		D3DXVECTOR2 end;

		numpadToCoords(a, start);
		numpadToCoords(b, end);

		float mx = end.x - start.x;
		float my = end.y - start.y;

		float angle = atan2(my, mx) + (3.1415926535f / 2.0f);

		//float lineBaseWidth = 6.0f;

		D3DXVECTOR2 offset(lineBaseWidth * cos(angle), lineBaseWidth * sin(angle));
		
		//const D3DXVECTOR2 lineTopLeft(9.0f / 16.0f, 7.0f / 16.0);
		//const D3DXVECTOR2 lineWidth(3.0f / 16.0f, 0.0f / 16.0f);
		const D3DXVECTOR2 lineTopLeft(10.0f / 16.0f, 7.0f / 16.0);
		const D3DXVECTOR2 lineWidth(1.0f / 16.0f, 0.0f / 16.0f);
		const D3DXVECTOR2 lineHeight(0.0f / 16.0f, 1.0f / 16.0f);

		// this should probs be done with 3 quads, 1 for each ball, 1 for line

		const Quad<MeltyVert> line(
			MeltyVert(start.x + offset.x, start.y + offset.y, lineTopLeft, colA),
			MeltyVert(start.x - offset.x, start.y - offset.y, lineTopLeft + lineHeight, colA),
			MeltyVert(end.x   + offset.x,   end.y + offset.y, lineTopLeft + lineWidth, colB),
			MeltyVert(end.x   - offset.x,   end.y - offset.y, lineTopLeft + lineWidth + lineHeight, colB)
		);

		meltyVertData.add(line);

		/*
		const Quad<MeltyVert> point1(
			MeltyVert(start.x + offset.x, start.y + offset.y, lineTopLeft, colA),
			MeltyVert(start.x - offset.x, start.y - offset.y, lineTopLeft + lineHeight, colA),
			MeltyVert(end.x + offset.x, end.y + offset.y, lineTopLeft + lineWidth, colA),
			MeltyVert(end.x - offset.x, end.y - offset.y, lineTopLeft + lineWidth + lineHeight, colA)
		);

		meltyVertData.add(point1);

		const Quad<MeltyVert> point2(
			MeltyVert(start.x + offset.x, start.y + offset.y, lineTopLeft, colB),
			MeltyVert(start.x - offset.x, start.y - offset.y, lineTopLeft + lineHeight, colB),
			MeltyVert(end.x + offset.x, end.y + offset.y, lineTopLeft + lineWidth, colB),
			MeltyVert(end.x - offset.x, end.y - offset.y, lineTopLeft + lineWidth + lineHeight, colB)
		);

		meltyVertData.add(point2);
		*/

		drawPoint(start.x, start.y, colA, size);
		drawPoint(end.x, end.y, colB, size);
	}

	void drawBounds() {
		
		const Quad<MeltyVert> bounds(
			MeltyVert(xPos - cornerScale * 1.1f, yPos - cornerScale * 1.1f),
			MeltyVert(xPos - cornerScale * 1.1f, yPos + cornerScale * 1.1f),
			MeltyVert(xPos + cornerScale * 3.15f, yPos - cornerScale * 1.1f),
			MeltyVert(xPos + cornerScale * 3.15f, yPos + cornerScale * 1.1f),
			0x60A0A0A0
		);
		
		meltyVertData.add(bounds);

		TextDraw(xPos + cornerScale * 3.15f - 14, yPos - cornerScale * 1.1f, 8, 0xFFFFFFFF, "P%d", player);

	}

	void drawBase() {

		const Quad<MeltyVert> center(
			MeltyVert(xPos - scale, yPos - scale),
			MeltyVert(xPos - scale, yPos + scale),
			MeltyVert(xPos + scale, yPos - scale),
			MeltyVert(xPos + scale, yPos + scale),
			0xA0A0A0A0
		);

		meltyVertData.add(center);

		const Tri<MeltyVert> corners[4] = {

			Tri<MeltyVert>(
				center.verts[0],
				center.verts[2],
				MeltyVert(xPos, yPos - cornerScale),
				0xA0A0A0A0
			),

			Tri<MeltyVert>(
				center.verts[1],
				center.verts[3],
				MeltyVert(xPos, yPos + cornerScale),
				0xA0A0A0A0
			),

			Tri<MeltyVert>(
				center.verts[0],
				center.verts[1],
				MeltyVert(xPos - cornerScale, yPos),
				0xA0A0A0A0
			),

			Tri<MeltyVert>(
				center.verts[2],
				center.verts[3],
				MeltyVert(xPos + cornerScale, yPos),
				0xA0A0A0A0
			)

		};

		meltyVertData.add(corners[0]);
		meltyVertData.add(corners[1]);
		meltyVertData.add(corners[2]);
		meltyVertData.add(corners[3]);

		drawPoint(xPos, yPos);

		drawLine(1, 2, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(2, 3, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(3, 6, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(6, 9, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(9, 8, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(8, 7, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(7, 4, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);
		drawLine(4, 1, 0xFFA0A0A0, 0xFFA0A0A0, 2.0f, 4.0f);

		drawPoint(xPos - cornerScale, yPos);
		drawPoint(xPos + cornerScale, yPos);
		drawPoint(xPos, yPos - cornerScale);
		drawPoint(xPos, yPos + cornerScale);

		drawPoint(xPos - scale, yPos - scale);
		drawPoint(xPos - scale, yPos + scale);
		drawPoint(xPos + scale, yPos - scale);
		drawPoint(xPos + scale, yPos + scale);

	}

	void drawLines() {

		// using the prev values might have weird consequences

		//const D3DXVECTOR2 lineTopLeft(9.0f / 16.0f, 7.0f / 16.0);
		//const D3DXVECTOR2 lineWidth(2.0f / 16.0f, 1.0f / 16.0f);
		//const D3DXVECTOR2 lineHeight(1.0f / 16.0f, 1.0f / 16.0f);

		MeltyVert v1;
		MeltyVert v2;

		DWORD colStart;
		DWORD colEnd;
		float t1;
		float t2;
	
		for (int i = -60; i <= 0; i++) {

			JoyLog curr = joyLog.get(i);
			JoyLog next = joyLog.get(i - 1);

			t1 = frame - curr.frame;
			if (t1 > 30.0f) {
				continue;
			}
			t2 = frame - next.frame;

			if (t2 > 30.0f) {
				t2 = 30.0f;
			}

			colStart = 0xFF000000 | ((BYTE)(fabs(-16.0f * t1 + 255.0f)) << 16) | ((BYTE)(-fabs(-16.0f * t1 + 255.0f) + 255.0f));
			colEnd = 0xFF000000 | ((BYTE)(fabs(-16.0f * t2 + 255.0f)) << 16) | ((BYTE)(-fabs(-16.0f * t2 + 255.0f) + 255.0f));

			drawLine(curr.dir, next.dir, colStart, colEnd);
		}
	}

	void drawJoystick() {

		//TextDraw(xPos, yPos, 16, 0xFFFFFFFF, "hi%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c", JOYSTICK, BUTTON_A, BUTTON_B, BUTTON_C, BUTTON_D, BUTTON_A_GRAY, BUTTON_B_GRAY, BUTTON_C_GRAY, BUTTON_D_GRAY, ARROW_1, ARROW_2, ARROW_3, ARROW_4, ARROW_6, ARROW_7, ARROW_8, ARROW_9);
		DWORD input = inputColumn->rawInputData[inputColumn->rawInputIndex];

		BYTE direction = input & 0xF;

		float joyX = xPos - 8;
		float joyY = yPos - 8;

		switch (direction) {
		default:
		case 0:
		case 5:
			break;
		case 1:
			joyX -= scale;
			joyY += scale;
			break;
		case 2:
			joyY += cornerScale;
			break;
		case 3:
			joyX += scale;
			joyY += scale;
			break;
		case 4:
			joyX -= cornerScale;
			break;
		case 6:
			joyX += cornerScale;
			break;
		case 7:
			joyX -= scale;
			joyY -= scale;
			break;
		case 8:
			joyY -= cornerScale;
			break;
		case 9:
			joyX += scale;
			joyY -= scale;
			break;
		}

		// todo, draw joystick afterimage
		if (joyX != prevJoyX || joyY != prevJoyY) {
		
			// a factor of 2 might not be the best here
			//prevJoyX = (joyX + prevJoyX) / 2.0f;
			//prevJoyY = (joyY + prevJoyY) / 2.0f;
			// also, putting the prevjoy thing before or after matters
			
			prevJoyX = (joyX * 0.45f) + (prevJoyX * 0.55f);
			prevJoyY = (joyY * 0.45f) + (prevJoyY * 0.55f);
			
			TextDraw(prevJoyX, prevJoyY, 16, 0x40FFFFFF, "%c", JOYSTICK);
		}

		TextDraw(joyX, joyY, 16, 0xFFFFFFFF, "%c", JOYSTICK);
		
		if (_naked_newPauseCallback2_IsPaused) {
			return;
		}

		if (joyLog.front().dir != direction) {
			//joyLog[joyLogIndex].dir = prevDir;
			//joyLog[joyLogIndex].frame = frame - 1;

			
			joyLog.pushHead(JoyLog(direction, frame));
			

			prevDir = direction;

		}
	}

	void drawButtons() {

		// tbh calling drawtext here is bad. im doing it anyway.

		BYTE buttonA = BUTTON_A_GRAY;
		BYTE buttonB = BUTTON_B_GRAY;
		BYTE buttonC = BUTTON_C_GRAY;
		BYTE buttonD = BUTTON_D_GRAY;
		BYTE buttonE = BUTTON_E_GRAY;
		BYTE buttonZ = BUTTON_DASH_GRAY;

		// 0xF00000 has buttons
		// 0xF00 has if a button was hit this turn 
		// 0xF has the direction.

		DWORD input = inputColumn->rawInputData[inputColumn->rawInputIndex];

		if ((input & 0x700000) == 0x700000) { // if E, dont handle the other buttons
			buttonE = BUTTON_E;
			sizeE = 22;
		} else {
			if ((input & 0x300000) == 0x300000) { // if dash, only handle C
				buttonZ = BUTTON_DASH;
				sizeZ = 22;
			} else { // handle A and B like normal
				if (input & 0x100000) {
					buttonA = BUTTON_A;
					sizeA = 22;
				}

				if (input & 0x200000) {
					buttonB = BUTTON_B;
					sizeB = 22;
				}
			}
			if (input & 0x400000) {
				buttonC = BUTTON_C;
				sizeC = 22;
			}
		}

		if (input & 0x800000) {
			buttonD = BUTTON_D;
			sizeD = 22;
		}
		
		TextDraw(xPos + cornerScale + 8 + (18 * 0) - ((sizeA - 16) >> 1), yPos - 16 - ((sizeA - 16) >> 1), sizeA, 0xFFFFFFFF, "%c", buttonA);
		TextDraw(xPos + cornerScale + 8 + (18 * 1) - ((sizeB - 16) >> 1), yPos - 16 - ((sizeB - 16) >> 1), sizeB, 0xFFFFFFFF, "%c", buttonB);
		TextDraw(xPos + cornerScale + 8 + (18 * 2) - ((sizeC - 16) >> 1), yPos - 16 - ((sizeC - 16) >> 1), sizeC, 0xFFFFFFFF, "%c", buttonC);
		TextDraw(xPos + cornerScale + 8 + (18 * 0) - ((sizeD - 16) >> 1), yPos +  2 - ((sizeD - 16) >> 1), sizeD, 0xFFFFFFFF, "%c", buttonD);
		TextDraw(xPos + cornerScale + 8 + (18 * 1) - ((sizeE - 16) >> 1), yPos +  2 - ((sizeE - 16) >> 1), sizeE, 0xFFFFFFFF, "%c", buttonE);
		TextDraw(xPos + cornerScale + 8 + (18 * 2) - ((sizeZ - 16) >> 1), yPos +  2 - ((sizeZ - 16) >> 1), sizeZ, 0xFFFFFFFF, "%c", buttonZ);

		if (sizeA > 16) { sizeA--; }
		if (sizeB > 16) { sizeB--; }
		if (sizeC > 16) { sizeC--; }
		if (sizeD > 16) { sizeD--; }
		if (sizeE > 16) { sizeE--; }
		if (sizeZ > 16) { sizeZ--; }
	}

	void draw() {
		if (!safeWrite() || isPaused()) {
			return;
		}
		handleDrag();
		drawBounds();
		drawBase();
		drawLines();
		drawJoystick();
		drawButtons();
		//drawLine(1, 9, 0xFFFF0000, 0xFF0000FF);
		if (_naked_newPauseCallback2_IsPaused) {
			return;
		}
		frame++;
	}

	float xPos = 300;
	float yPos = 300;
	const float scale = 50.0f;
	const float cornerScale = 60.0f;

	int sizeA = 16;
	int sizeB = 16;
	int sizeC = 16;
	int sizeD = 16;
	int sizeE = 16;
	int sizeZ = 16;

	float prevJoyX = xPos - 8;
	float prevJoyY = yPos - 8;
	int prevDir = 0;

	//JoyLog joyLog[60];
	//int joyLogIndex = 0;
	CircularBuffer<JoyLog, 60> joyLog;
	unsigned frame = 0;

	InputColumn* inputColumn = NULL;
	int player;

	static int hasDrag;

};

int InputDisplay::hasDrag = 0;

//200.0f, 400.0f
//378.0f, 400.0f

InputDisplay P1InputDisplay(200.0f - 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P1InputBar, 1);
InputDisplay P2InputDisplay(378.0f + 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P2InputBar, 2);

void drawFancyInputDisplay() {

	P1InputDisplay.draw();
	P2InputDisplay.draw();

}
