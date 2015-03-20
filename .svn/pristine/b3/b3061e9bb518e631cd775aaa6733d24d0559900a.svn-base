#pragma once

//#ifndef WIN32
	#include <windows.h>
//#endif

#include "Util.h"
#include <gtk/gtk.h>
#include <vector>
#include <map>

#include <GL/gl.h>
#include <GL/glu.h>

class GuiClass
{
public:
	GuiClass(char *filename);
	~GuiClass(void);
private:
	static map<char*,widgetData*> _widgetMap;
	GtkBuilder *_builder;
	void loadGuiFile(char *guiFilename);
	void init(char *guiFilename);
	void setGuiWidgets();
	void connectCallbacks();
	void addIdleFunctions();
	void showMainWindowAndBlock();
	/*static funtions*/
	static void button_clicked(GtkWidget *widget, gpointer data);
	static void setWidgetColor(GtkWidget *widget, vector<int> colorVector);
	static void setWidgetColor(GtkWidget *widget, string colorName);
	static GdkColor *getGdkColor(int red, int green, int blue);
	static gboolean idleFuncLAbel(widgetData *widgetData);
	static gboolean idleFuncProgressBar(widgetData *widgetData);
	static widgetData *getNewWidgetData(GtkBuilder * builder, char *widgetName);
	static void setLabelColoredFlag(GtkWidget *widget, bool state, char *onString, char *offString);
	static void setComboBoxItems(GtkComboBox *comboBox, int arrayLength, char** stringArray);
	static void comboBox_Change(GtkComboBox *combo, gpointer callData);
	static gboolean setDrawingArea(widgetData *widgetData);
	static GdkGC *getGColor(GdkDrawable *drawable, int red, int green, int blue);
	static void drawMotionPlusAngleLine(GdkWindow *window, GdkGC *color, double angleRadians, int lenghtofline, int lineThick);
};
