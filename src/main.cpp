#include "cubeui.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	CubeUi window;

	window.setWindowTitle("qubetimer");
	window.show();

	return app.exec();
}
