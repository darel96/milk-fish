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

	void DrawShotPoint(double x, double y);
};

Target target;

class Runner
{
private:
	double middle = pixelQuantity / 2;
	double lowerOverlimitCoefficient = 1.6;
	double upperOverlimitCoefficient = 1.3;

	double upperRunnerCoordinate = middle + upperOverlimitCoefficient * target.doubleRadius;
	double Step = pixelQuantity / 80;

public:
	double AdditionalCoordinate = pixelQuantity / 15;
	double runnerCoordinate = lowerRunnerCoordinate + 100;
	double lowerRunnerCoordinate = middle - lowerOverlimitCoefficient *  target.doubleRadius;

	bool horisontalRun = true;

	double VerticalRunner();
	double HorisontalRunner();

	void DelayRunner(double runnerCoordinate, double AdditionalCoordinate, double lowerRunnerCoordinate);
};

Runner runner;

int maxShot = 6;
int shotCount = 0;

struct Coordinates
{
	double x = -1, y = -1;
};

Coordinates *points = new Coordinates[maxShot];
int lastDefinitePoint  = 0;

double runnerCoordinate = 0,
AdditionalCoordinate = 0,
lowerRunnerCoordinate = 0;

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	target.DrawTarget();

	for (int i = 0; i < lastDefinitePoint; i++)
	{
		target.DrawShotPoint(points[i].x, points[i].y);
	}

	if (runner.horisontalRun)
	{
		points[lastDefinitePoint].x = runner.HorisontalRunner();

		runnerCoordinate = runner.runnerCoordinate;
		AdditionalCoordinate = runner.AdditionalCoordinate;
		lowerRunnerCoordinate = runner.lowerRunnerCoordinate;
	}
	else
	{
		points[lastDefinitePoint].y = runner.VerticalRunner();
		
		runner.DelayRunner(runnerCoordinate, AdditionalCoordinate, lowerRunnerCoordinate);
	}

	cout << 'x' << points[lastDefinitePoint].x << endl << 'y' << points[lastDefinitePoint].y << endl;
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
	glutDisplayFunc(Display);
	glutTimerFunc(10, TimerFunction, 1);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SKeyboard);
	glutMainLoop();
}

int main(int argc, char **argv)
{
	init(argc, argv);
}


void DrawArc(int currentPart, double radius);

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


#define PI 3.1415926535898

void DrawCircle(double radius);

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

void Target::DrawShotPoint(double x, double y)
{
	glColor3f(1, 0, 1);
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
					if (lastDefinitePoint == maxShot)
					{
						lastDefinitePoint = 0;
					}

					runner.horisontalRun = false;
				}
				else
				{

					shotCount += 1;
					runner.horisontalRun = true;
					lastDefinitePoint++;

				}

				if (shotCount == maxShot)
				{
					cout << endl << "Your turn is finished.";
					shotCount = 0;
				}
	}
		break;
	default:
		break;
	}
}

void SKeyboard(int key, int x, int y)
{

}

double Runner::VerticalRunner()
{
	runnerCoordinate += Step;

	if (runnerCoordinate >= upperRunnerCoordinate || runnerCoordinate <= lowerRunnerCoordinate)
	{
		Step = -Step;
	}

	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
		glVertex2f(lowerRunnerCoordinate, runnerCoordinate);
		glVertex2f(lowerRunnerCoordinate + AdditionalCoordinate, runnerCoordinate + AdditionalCoordinate / 4);
		glVertex2f(lowerRunnerCoordinate, runnerCoordinate + AdditionalCoordinate / 2);
	glEnd();
	glFlush();

	return (runnerCoordinate + AdditionalCoordinate / 4);
}

double Runner::HorisontalRunner()
{
	runnerCoordinate += Step;

	if (runnerCoordinate >= upperRunnerCoordinate || runnerCoordinate <= lowerRunnerCoordinate)
	{
		Step = -Step;
	}

	glColor3f(1.0, 0.0, 0.0);
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
