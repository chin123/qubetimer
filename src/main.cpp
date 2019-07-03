#include "cubeui.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);  

	CubeUi window;

	window.resize(300, 190);
	window.setWindowTitle("Rubik's Cube Timer");
	window.show();

	return app.exec();
}
