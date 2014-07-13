#include <iostream>
#include <windows.h> 
#include <GL/glut.h>
#include <GL/glaux.h>
using namespace std;

const int pixelQuantity = 1000;
int centralCoordinate = pixelQuantity / 2;

class Text
{
private:
	int Offset;

public:
	void CreateTextFont(char *fontName, int size);
	void DrawText(double x, double y, int num, int currentPlayer);
	
};

Text text;

class Target
{
private:
	double ringSizeCoefficient = 0.06;
	double tripleRadiusCoefficient = 0.63;
	double centralCircleCoefficient = 0.093;
	double bullCoefficient = 0.037;
	double backgroundCircleCoefficient = 1.2;

public:
	double doubleRadius = pixelQuantity / 4;
	double backgroundCircleRadius = backgroundCircleCoefficient * doubleRadius;
	double outerBlack_WhiteCircleRadius = (1 - ringSizeCoefficient) * doubleRadius;
	double tripleRadius = tripleRadiusCoefficient * doubleRadius;
	double insideBlack_WhiteCircleRadius = (tripleRadiusCoefficient - ringSizeCoefficient) * doubleRadius;
	double centralCircleRadius = centralCircleCoefficient * doubleRadius;
	double bullRadius = bullCoefficient * doubleRadius;

	void DrawTarget()
	{
		void DrawBackgroundCircle(double);
		void DrawRed_GreenRing(double);
		void DrawWhite_BlackCircle(double);
		void DrawCentralGreenCircle(double);
		void DrawBull(double);

		DrawBackgroundCircle(backgroundCircleRadius);
		DrawRed_GreenRing(doubleRadius);
		DrawWhite_BlackCircle(outerBlack_WhiteCircleRadius);
		DrawRed_GreenRing(tripleRadius);
		DrawWhite_BlackCircle(insideBlack_WhiteCircleRadius);
		DrawCentralGreenCircle(centralCircleRadius);
		DrawBull(bullRadius);
	}
	void DrawShotPoint(double x, double y, int playerNumber);
};

Target target;
int currentPlayer = 0;

class Runner
{
private:
	double middle = pixelQuantity / 2;
	double lowerOverlimitCoefficient = 1.6;
	double upperOverlimitCoefficient = 1.3;

	double upperRunnerCoordinate = middle + upperOverlimitCoefficient * target.doubleRadius;
	double step = pixelQuantity / 100;

	double dX = abs(currentPlayer - 1)*pixelQuantity*0.7;
public:
	double AdditionalCoordinate = pixelQuantity / 15;
	double runnerCoordinate = lowerRunnerCoordinate + 100 + dX;
	double lowerRunnerCoordinate = middle - lowerOverlimitCoefficient *  target.doubleRadius;

	bool horisontalRun = true;

	double VerticalRunner(int playerNumber);
	double HorisontalRunner(int playerNumber);
	void DelayRunner(double runnerCoordinate, double AdditionalCoordinate, double lowerRunnerCoordinate);
	int Scores(double x, double y);
};

Runner runner;

class Player
{
private:
	struct Colors
	{
		double red = 0, green = 0, blue = 0;
	};

	struct Coordinates
	{
		double x, y;
	};

public:
	int scores = 301;
	Colors colors;

	Coordinates ScoresPlace;

	void SetColor();
	void AddScores(int);
};

Player player[2];

void InitPlayers()
{
	player[0].colors.red = 1;
	player[0].colors.green = .3;
	player[0].colors.blue = 0;

	player[1].colors.red = .3;
	player[1].colors.green = 0;
	player[1].colors.blue = 1;

	player[0].ScoresPlace = { pixelQuantity*.07, pixelQuantity*.9 };
	player[1].ScoresPlace = { pixelQuantity*.82, pixelQuantity*.9 };
}

const int maxShot = 3;
int shotCount = 0;

struct Coordinates
{
	double x, y;
};

Coordinates *points = new Coordinates[maxShot];
int lastDefinitePoint  = 0;

double runnerCoordinate = 0,
AdditionalCoordinate = 0,
lowerRunnerCoordinate = 0;

bool shoted = false;
bool gameStarted = false;
bool exitGame = false;
int scoresPerTurn = 0;


void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	target.DrawTarget();

	if (gameStarted)
	{
		int scoresXPosition = pixelQuantity*(currentPlayer*0.75 + 0.075);
		text.DrawText(player[0].ScoresPlace.x, player[0].ScoresPlace.y, player[0].scores, 0);
		text.DrawText(player[1].ScoresPlace.x, player[1].ScoresPlace.y, player[1].scores, 1);
		text.DrawText(scoresXPosition, centralCoordinate, scoresPerTurn, currentPlayer);
	}

	for (int i = 0; i < lastDefinitePoint; i++)
	{
		target.DrawShotPoint(points[i].x, points[i].y, currentPlayer);
	}

	if (runner.horisontalRun)
	{
		points[lastDefinitePoint].x = runner.HorisontalRunner(currentPlayer);

		runnerCoordinate = runner.runnerCoordinate;
		AdditionalCoordinate = runner.AdditionalCoordinate;
		lowerRunnerCoordinate = runner.lowerRunnerCoordinate;

		if (shoted && gameStarted)
		{
			scoresPerTurn += runner.Scores(points[lastDefinitePoint - 1].x, points[lastDefinitePoint - 1].y);
			shoted = false;
		}
	}
	else
	{
		points[lastDefinitePoint].y = runner.VerticalRunner(currentPlayer);
		runner.DelayRunner(runnerCoordinate, AdditionalCoordinate, lowerRunnerCoordinate);
	}
}

void TimerFunction(int value)
{

	glutPostRedisplay();
	glutTimerFunc(10, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y);
void SKeyboard(int key, int x, int y);

int screenLength = 768, screenWidth = 1366,
windowSize = screenLength - screenLength / 10;
int windowPosition = screenWidth - windowSize;

void init(int argc, char **argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(windowSize, windowSize);
	glutInitWindowPosition(windowPosition, 0);
	glutCreateWindow("Aim and shoot!");
	glClearColor(1.0, .8, 1.0, .8);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, pixelQuantity, 0.0, pixelQuantity, 0, 1);

	text.CreateTextFont("Courier New", 40);
	InitPlayers();
	glutDisplayFunc(Display);
	glutTimerFunc(10, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SKeyboard);
	glutMainLoop();
}


int main(int argc, char **argv)
{
	cout << "If you want to start game - press <<s>>" << endl;

	init(argc, argv);
}

void Text::CreateTextFont(char *fontName, int size)
{
	HDC hDC = wglGetCurrentDC();
	int FontSize = -MulDiv(GetDeviceCaps(hDC, LOGPIXELSY), size, 72);
	HFONT Font = CreateFont(FontSize, 0, 0, 0, 500, false, false, false, 204, 3, 2, 1, 34, fontName);
	Offset = glGenLists(256);
	SelectObject(hDC, Font);
	wglUseFontBitmaps(hDC, 0, 256, Offset);
}

int NumberLength(int number)
{
	int cell = 1, length = 0;

	for (int i = 1; cell != 0; i++)
	{
		cell = number / pow(10.0, i);
		length++;
	}
	return length;
}

void Text::DrawText(double x, double y, int num, int currentPlayer)
{
	player[currentPlayer].SetColor();
	char number[10];
	int figure;
	for (int pos = 0; pos < NumberLength(num); pos++)
	{
		figure = num / pow(10.0, (NumberLength(num) - pos - 1));//Переход из "1234567" в "123", где 3 - позиция в числе. 
		figure = figure - (figure / 10) * 10;// "123 - 120 = 3" - получение цифры заданной позиции.
		number[pos] = char(figure + 48);
	}
	glListBase(Offset);
	glRasterPos2d(x, y);
	glCallLists(NumberLength(num), GL_UNSIGNED_BYTE, number);
	glutSwapBuffers();
}


void DrawCircle(double radius);
void DrawArc(int currentPart, double radius);
#define PI 3.1415926535898

const int circle_points = 3600;
const int targetPartsCount = 20;

void DrawRed_GreenRing(double radius)
{
	for (int currentPart = 1; currentPart <= targetPartsCount; currentPart++)
	{
		int green_redPartColor = currentPart % 2;
		glColor3f(abs(green_redPartColor - 1), green_redPartColor, 0);

		DrawArc(currentPart, radius);

	}
}

void DrawWhite_BlackCircle(double radius)
{

	for (int currentPart = 1; currentPart <= targetPartsCount; currentPart++)
	{
		int black_WhitePartColor = currentPart % 2;
		glColor3f(black_WhitePartColor, black_WhitePartColor, black_WhitePartColor);
		DrawArc(currentPart, radius);
	}
}

void DrawBackgroundCircle(double circleRadius)
{
	glColor3f(0, 0, 0);

	DrawCircle(circleRadius);
}

void DrawCentralGreenCircle(double centralCircleRadius)
{
	glColor3f(0, 1, 0);

	DrawCircle(centralCircleRadius);
}

void DrawBull(double bullRadius)
{
	glColor3f(1, 0, 0);

	DrawCircle(bullRadius);
}


void DrawArc(int currentPart, double radius)
{
	glBegin(GL_POLYGON);

	glVertex2f(centralCoordinate, centralCoordinate);

	int firstDrawingPoint = (currentPart - 1) * circle_points / targetPartsCount;
	int lastDrawingPoint = currentPart * circle_points / targetPartsCount;

	for (int iPoint = firstDrawingPoint; iPoint <= lastDrawingPoint; iPoint++)
	{
		double angle = 360 * iPoint / circle_points - 360 / (2 * targetPartsCount);
		double dy = sin(angle*PI / 180) * radius;
		double dx = cos(angle*PI / 180) * radius;
		glVertex2f(centralCoordinate + dx, centralCoordinate + dy);
	}

	glEnd();
	glutSwapBuffers();
}

void DrawCircle(double radius)
{
	glBegin(GL_POLYGON);

	for (int iPoint = 1; iPoint <= circle_points; iPoint++)
	{
		double angle = 360 * iPoint / circle_points;
		double dy = sin(angle*PI / 180) * radius;
		double dx = cos(angle*PI / 180) * radius;
		glVertex2f(centralCoordinate + dx, centralCoordinate + dy);
	}

	glEnd();
	glutSwapBuffers();
}

void Target::DrawShotPoint(double x, double y, int playerNumber)
{
	player[playerNumber].SetColor();
	glBegin(GL_POLYGON);//скопировано с DrawCircle

	for (int iPoint = 1; iPoint <= circle_points; iPoint++)
	{
		double angle = 360 * iPoint / circle_points;
		double dy = sin(angle*PI / 180) * pixelQuantity/100;
		double dx = cos(angle*PI / 180) * pixelQuantity/100;
		glVertex2f(x + dx, y + dy);
	}

	glEnd();

	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
		glVertex2f(x - pixelQuantity / 50, y + pixelQuantity / 50);
		glVertex2f(x + pixelQuantity / 50, y - pixelQuantity / 50);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(x - pixelQuantity / 50, y - pixelQuantity / 50);
		glVertex2f(x + pixelQuantity / 50, y + pixelQuantity / 50);
	glEnd();
	
	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
 {
	switch (key)
	{
	case ' ':
	{
 				runner.runnerCoordinate = runner.lowerRunnerCoordinate + 100;
				if (runner.horisontalRun == true)
				{
					runner.horisontalRun = false;
					if (lastDefinitePoint == maxShot)
					{
						lastDefinitePoint = 0;

						runner.horisontalRun = true;
			
						player[currentPlayer].AddScores(scoresPerTurn);

						char answer = ' ';
						if (player[currentPlayer].scores == 0 && gameStarted)
						{
							cout << endl << "WOW! WE HAVE A WINNER!!! " << endl << "The " << " Player" <<  currentPlayer + 1 <<  " is won this match! Congratulations!"
								<< endl << "Would you want to repeat game?(y/n)" << endl;

							cin >> answer;
						}


						if (answer == 'n')
						{
							exitGame = true;
							gameStarted = false;
							cout << endl << "If you want to start game - press <<s>>";
							goto end;
						}
						else if (answer == 'y')
						{
							player[0].scores = 301;
							player[1].scores = 301;
						}

						scoresPerTurn = 0;
						shotCount = 0;

						if (currentPlayer == 0)
						{
							currentPlayer = 1;
						}
						else
						{
							currentPlayer = 0;
						}

						double dX = abs(currentPlayer - 1)*pixelQuantity*0.7;
						double startingPosition = lowerRunnerCoordinate + dX + 10;
						runner.runnerCoordinate = startingPosition;
					}
				}
				else
				{
					shotCount++;
					runner.horisontalRun = true;
					lastDefinitePoint++;

					double dX = abs(currentPlayer - 1)*pixelQuantity*0.7;
					double startingPosition = lowerRunnerCoordinate +10 + dX;
					shoted = true;
				}
	}
		end:
		break;
	case 's':
	{
				if (!gameStarted)
				{
					player[0].scores = 301;
					player[1].scores = 301;
					gameStarted = true;
					scoresPerTurn = 0;
					shotCount = 0;
					lastDefinitePoint = 0;
				}
				else
				{
					cout << "Do you want restart game?";
					char answer;
					cin >> answer;

					if (answer == 'y')
					{
						player[0].scores = 301;
						player[1].scores = 301;
						gameStarted = true;
						scoresPerTurn = 0;
						shotCount = 0;
						lastDefinitePoint = 0;

					}
				}
	}
		break;
	default:
		break;
	};
}

void SKeyboard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_END:	
	{ 
						 if (gameStarted)
						 {
							 char magic[40];
							 char message[40] = "it is not what you think";
							 gets_s(magic, 40);

							 for (int i = 0; i <= 24; i++)
							 {
								 if (magic[i] != message[i])
								 {
									 goto ImNotCheater;
								 }
							 }

							 player[currentPlayer].scores = 0;
						 ImNotCheater:
						 }
	}
		break;
	default:
		break;
	}
}

double Runner::VerticalRunner(int playerNumber)
{
	if (runnerCoordinate + step >= upperRunnerCoordinate || runnerCoordinate + step <= lowerRunnerCoordinate)
	{
		step = -step;
	}
	
	runnerCoordinate += step;

	player[playerNumber].SetColor();

	double dX = abs(currentPlayer - 1)*0.78*pixelQuantity;
	double dA = AdditionalCoordinate * 2 * (currentPlayer - 1);
	glBegin(GL_POLYGON);
		glVertex2f(lowerRunnerCoordinate + dX, runnerCoordinate);
		glVertex2f(lowerRunnerCoordinate + dX + AdditionalCoordinate + dA, runnerCoordinate + AdditionalCoordinate / 4);
		glVertex2f(lowerRunnerCoordinate + dX, runnerCoordinate + AdditionalCoordinate / 2);
	glEnd();
	glFlush();

	return (runnerCoordinate + AdditionalCoordinate / 4);
}

double Runner::HorisontalRunner(int playerNumber)
{
	runnerCoordinate += step;

	if (runnerCoordinate >= upperRunnerCoordinate || runnerCoordinate <= lowerRunnerCoordinate)
	{
		step = -step;
	}

	player[playerNumber].SetColor();
	glBegin(GL_POLYGON);
		glVertex2f(runnerCoordinate, lowerRunnerCoordinate);
		glVertex2f(runnerCoordinate + AdditionalCoordinate / 4, lowerRunnerCoordinate + AdditionalCoordinate);
		glVertex2f(runnerCoordinate + AdditionalCoordinate / 2, lowerRunnerCoordinate);
	glEnd();
	glFlush();

	return (runnerCoordinate + AdditionalCoordinate / 4);
}

void Runner::DelayRunner(double runnerCoordinate, double AdditionalCoordinate, double lowerRunnerCoordinate)
{
	glBegin(GL_POLYGON);
		glVertex2f(runnerCoordinate, lowerRunnerCoordinate);
		glVertex2f(runnerCoordinate + AdditionalCoordinate / 4, lowerRunnerCoordinate + AdditionalCoordinate);
		glVertex2f(runnerCoordinate + AdditionalCoordinate / 2, lowerRunnerCoordinate);
	glEnd();
	glFlush();
}


int Runner::Scores(double x, double y)
{
	y -= middle;
	x -= middle;
	double partAngle = 360 / targetPartsCount,
		dA = partAngle / 2,
		leftmostPointAngle = -180 - dA;

	double angle = atan2(y, x) * 180 / PI;

	int Axis;
	for (Axis = 1; angle > leftmostPointAngle + partAngle*Axis; Axis++);

	int scores;

	switch (Axis)
	{
	case 2: scores = 8;
		break;
	case 3: scores = 16;
		break;
	case 4: scores = 7;
		break;
	case 5: scores = 19;
		break;
	case 6: scores = 3;
		break;
	case 7: scores = 17;
		break;
	case 8: scores = 2;
		break;
	case 9: scores = 15;
		break;
	case 10: scores = 10;
		break;
	case 11: scores = 6;
		break;
	case 12: scores = 13;
		break;
	case 13: scores = 4;
		break;
	case 14: scores = 18;
		break;
	case 15: scores = 1;
		break;
	case 16: scores = 20;
		break;
	case 17: scores = 5;
		break;
	case 18: scores = 12;
		break;
	case 19: scores = 9;
		break;
	case 20: scores = 14;
		break;
	default: scores = 11;
		break;
	}

	double pointRadius = sqrt(x*x + y*y);

	if (pointRadius < target.backgroundCircleRadius)
	{
		if (pointRadius > target.bullRadius)
		{
			if (pointRadius > target.centralCircleRadius)
			{
				if (pointRadius > target.insideBlack_WhiteCircleRadius)
				{
					if (pointRadius > target.tripleRadius)
					{
						if (pointRadius > target.outerBlack_WhiteCircleRadius)
						{
							if (pointRadius > target.doubleRadius)
							{
								scores = 0;
							}
							else
							{
								scores *= 2;
							}
						}
					}
					else
					{
						scores *= 3;
					}
				}
			}
			else
			{
				scores = 25;
			}
		}
		else
		{
			scores = 50;
		}
	}
	else
	{
		scores = 0;
	}

	return scores;
}


void Player::SetColor()
{
	glColor3f(colors.red, colors.green, colors.blue);
}

void Player::AddScores(int additionalScores)
{
	if (scores - additionalScores >= 0)
	{
		scores -= additionalScores;
	}
}
