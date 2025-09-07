



#include "Globals.h"

//ManPageEditorQT	*mpclass=NULL;

int main(int argc, char **argv)
{
	int				status;
	QApplication		app(argc,argv);
	QPixmap			pixmap(DATADIR "/pixmaps/KKEditQT.png");

	app.setOrganizationName("KDHedger");
	app.setApplicationName("ManPageEditorQT");

	mpclass=new ManPageEditorQT(&app);
    mpclass->splash=new QSplashScreen(pixmap,Qt::FramelessWindowHint|Qt::X11BypassWindowManagerHint);

	status=app.exec();

	delete mpclass;
	return status;
}