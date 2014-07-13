#include <iostream>
#include <GL/glut.h>
using namespace std;

const int pixelQuantity = 1000;

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

class Runner
{
private:
	double middle = pixelQuantity / 2;
	double lowerOverlimitCoefficient = 1.6;
	double upperOverlimitCoefficient = 1.3;

	double upperRunnerCoordinate = middle + upperOverlimitCoefficient * target.doubleRadius;
	double Step = pixelQuantity / 100;

public:
	double AdditionalCoordinate = pixelQuantity / 15;
	double runnerCoordinate = lowerRunnerCoordinate + 100;
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

public:
	int scores = 301;
	Colors colors;

	void SetColor();
	void AddScores(int);
};

Player player[2];

void InitPlayersColors()
{
	player[0].colors.red = 1;
	player[0].colors.green = .3;
	player[0].colors.blue = 0;

	player[1].colors.red = .3;
	player[1].colors.green = 0;
	player[1].colors.blue = 1;
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
bool startGame = false;
bool exitGame = false;

int currentPlayer = 0;
int scoresPerTurn = 0;


void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	target.DrawTarget();

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

		if (shoted && startGame)
		{
			cout << runner.Scores(points[lastDefinitePoint - 1].x, points[lastDefinitePoint - 1].y) << ' ';
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

	InitPlayersColors();
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

void DrawCircle(double radius);
void DrawArc(int currentPart, double radius);
#define PI 3.1415926535898

const int circle_points = 3600;
const int targetPartsCount = 20;
int centralCoordinate = pixelQuantity / 2;

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
 				runner.runnerCoordinate = runner.lowerRunnerCoordinate + 1;
				if (runner.horisontalRun == true)
				{
					runner.horisontalRun = false;
					if (lastDefinitePoint == maxShot)
					{
						lastDefinitePoint = 0;

						runner.horisontalRun = true;
			
						player[currentPlayer].AddScores(scoresPerTurn);
						
						char answer = ' ';
						if (player[currentPlayer].scores == 0 && startGame)
						{
							cout << endl << "WOW! WE HAVE A WINNER!!! " << endl << "The " << " Player" <<  currentPlayer + 1 <<  " is won this match! Congratulations!"
								<< endl << "Would you want to repeat game?(y/n)" << endl;

							cin >> answer;
						}


						if (answer == 'n')
						{
							exitGame = true;
							startGame = false;
							cout << endl << "If you want to start game - press <<s>>";
							
							player[0].scores = 301;
							player[1].scores = 301;
							goto end;
						}
						else if (answer == 'y')
						{
							player[0].scores = 301;
							player[1].scores = 301;
						}

						scoresPerTurn = 0;
						
						if (startGame)
						{
							cout << endl << "You have " << player[currentPlayer].scores << " scores. Your turn is finished." <<
								endl << "Began the " << "Player" << currentPlayer + 1 << "'s turn." << endl;
						}

						shotCount = 0;

						if (currentPlayer == 0)
						{
							currentPlayer = 1;
						}
						else
						{
							currentPlayer = 0;
						}
					}
				}
				else
				{
					shotCount++;
					runner.horisontalRun = true;
					lastDefinitePoint++;

					shoted = true;
				}
	}
		end:
		break;
	case 's': 
		startGame = true;
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
						 char magic[40];
						 char message[40] = "It is not cheating - it's MAGIC...";
						 gets_s(magic, 40);

							 if (magic == message)
							 {
								 goto ImNotCheater;
							 }
						 player[currentPlayer].scores = 0;

					 ImNotCheater:;
	}
		break;
	default:
		break;
	}
}

double Runner::VerticalRunner(int playerNumber)
{
	runnerCoordinate += Step;

	if (runnerCoordinate >= upperRunnerCoordinate || runnerCoordinate <= lowerRunnerCoordinate)
	{
		Step = -Step;
	}

	player[playerNumber].SetColor();

	glBegin(GL_POLYGON);
		glVertex2f(lowerRunnerCoordinate, runnerCoordinate);
		glVertex2f(lowerRunnerCoordinate + AdditionalCoordinate, runnerCoordinate + AdditionalCoordinate / 4);
		glVertex2f(lowerRunnerCoordinate, runnerCoordinate + AdditionalCoordinate / 2);
	glEnd();
	glFlush();

	return (runnerCoordinate + AdditionalCoordinate / 4);
}

double Runner::HorisontalRunner(int playerNumber)
{
	runnerCoordinate += Step;

	if (runnerCoordinate >= upperRunnerCoordinate || runnerCoordinate <= lowerRunnerCoordinate)
	{
		Step = -Step;
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