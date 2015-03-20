#pragma once
#include <yarp/dev/WiimoteInterface.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <math.h>
#include "utils.h"

#include <queue>


#define PI 3.14159265

using namespace std;
using namespace yarp::dev;
using namespace yarp::sig;

class Gui
{
public:
	Gui(IWiimote *wiiSensor);
	~Gui(void);
	void gladeCartesianGui();
private:
	static IWiimote *_wiiSensor;
	static GtkBuilder *_builder;
	string _guiFile;
	GtkEntry *getEntry(string entryName);
	static GtkLabel *getLabel(string labelName);
	static GObject *getGObj(string gobjName);
	static void setWidgetColor(GtkWidget *widget, string colorName);
	static gboolean setLabel(GtkLabel *label);
	static gboolean windowCycle(GtkWindow *window);
	static gboolean setDrawingArea(GtkDrawingArea *drawArea);
	static gboolean setMotionPlusCurve(GtkCurve *curveArea);
	static gboolean idleStr(gchar *str);
	static GdkGC *getGColor(GdkDrawable *drawable, int red, int green, int blue);
};