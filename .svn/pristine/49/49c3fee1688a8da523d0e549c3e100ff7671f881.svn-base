#include "Gui.h"

//static members
IWiimote *Gui::_wiiSensor;
GtkBuilder *Gui::_builder;

//GUI CLASS FUNCTIONS
Gui::Gui(IWiimote *wiiSensor)
{
	_builder = NULL;
	_guiFile = "../gui/wiiGui.glade";
	_wiiSensor = wiiSensor;
}

Gui::~Gui(void)
{
	/* Destroy builder */
	g_object_unref( G_OBJECT( _builder ) );
}

void Gui::gladeCartesianGui(){
	GtkBuilder *builder;
	GtkWidget  *window;

	gtk_init( NULL, NULL );

	/* Create builder and load interface */
	builder = gtk_builder_new();
	_builder = builder;
	int builderOK = gtk_builder_add_from_file( builder, _guiFile.c_str(), NULL );
	if(builderOK) printf("builder ok!\n");
	else printf("builder error!\n");

	/* Obtain widgets that we need */
	window = GTK_WIDGET( gtk_builder_get_object( builder, "window1" ) );

	/* Connect callbacks */
	gtk_builder_connect_signals( builder, NULL );

	/* Destroy builder */
	//g_object_unref( G_OBJECT( builder ) );

	/* Show main window and start main loop */
	gtk_widget_show( window );

	//GTK IDLE FUNCTIONS
	g_idle_add((GSourceFunc)Gui::windowCycle, GTK_WINDOW(getGObj("window1")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accX")));
	g_idle_add((GSourceFunc)Gui::idleStr,"accX");
	/*g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accY")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accZ")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpX")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpY")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpZ")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpDiffX")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpDiffY")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpDiffZ")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpRoll")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpPitch")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("mpYaw")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accRoll")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accPitch")));
	g_idle_add((GSourceFunc)Gui::setLabel, GTK_LABEL(getGObj("accYaw")));
	g_idle_add((GSourceFunc)Gui::setDrawingArea, GTK_DRAWING_AREA(getGObj("pitchMeter")));
*/
	gtk_main();

}

gboolean Gui::windowCycle(GtkWindow *window){
	_wiiSensor->wiiRefresh();
	if(_wiiSensor->wiiGetBotState(0,_wiiSensor->A))
		_wiiSensor->wiiSetParts(0,1,_wiiSensor->MOTION_PLUS);
	if(_wiiSensor->wiiGetBotState(0,_wiiSensor->B))
		_wiiSensor->wiiSetParts(0,0,_wiiSensor->MOTION_PLUS);
	if(_wiiSensor->wiiGetBotState(0,_wiiSensor->ONE))
		_wiiSensor->wiiSetRumble(0,true);
	if(_wiiSensor->wiiGetBotState(0,_wiiSensor->TWO))
		_wiiSensor->wiiSetRumble(0,false);
	if(_wiiSensor->wiiGetBotState(0,_wiiSensor->HOME))
		_wiiSensor->wiiSetLed(0,_wiiSensor->LED_THREE+_wiiSensor->LED_TWO);
	return true;
}


gboolean Gui::setDrawingArea(GtkDrawingArea *drawArea)
{
	// set up colors and some GCs
	GdkWindow	*window = drawArea->widget.window;
	int width = drawArea->widget.allocation.width;
	int height = drawArea->widget.allocation.height;
	int circleRadius = width<height?width:height;
	int lineThick = 5;
	int initX, initY, endX, endY;
	GdkGC		*bg_gc = getGColor(window,65535,65535,65535);
	GdkGC		*green_gc = getGColor(window,0,65535,0);
	GdkGC		*black_gc = getGColor(window,0,0,0);
	gdk_gc_set_line_attributes(green_gc,lineThick,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_ROUND);

	gdk_draw_arc (window,bg_gc,TRUE, 0, 0, circleRadius, circleRadius,0, 360*64);

	Vector motionPlusVector;
	motionPlusVector.resize(3);
	_wiiSensor->readMotionPlusAngle(0,motionPlusVector);
	double angleRadians = motionPlusVector[0] * (PI/180);
	initX = (int)(sin(angleRadians)*circleRadius/2)+circleRadius/2;
	initY = (int)(cos(angleRadians)*circleRadius/2)+circleRadius/2;
	endX = (int)(sin(angleRadians + PI)*circleRadius/2)+circleRadius/2;
	endY = (int)(cos(angleRadians + PI)*circleRadius/2)+circleRadius/2;
	gdk_draw_line (window, green_gc, initX, initY, endX, endY);

	gdk_draw_arc(window, black_gc, TRUE, circleRadius/2,circleRadius/2, lineThick, lineThick, 0, 360*64);


	return true;
}

GdkGC *Gui::getGColor(GdkDrawable *drawable, int red, int green, int blue){
	GdkColor color;
	GdkGC *color_gc = gdk_gc_new(drawable);
	//0 to 65535
	color.red = red;
	color.green = green;
	color.blue = blue;
	gdk_gc_set_rgb_fg_color( color_gc, &color);
	return color_gc;
}

gboolean Gui::idleStr(gchar *str){
	printf("idleStr: %s\n",str);
	return true;
}

gboolean Gui::setLabel(GtkLabel *label)
{
	char *labelName = label->misc.widget.name;
	printf("%s \n",labelName);
	if(labelName == NULL) return true;
	if(strncmp(labelName,"acc",3)==0){
		Vector accPos;
		if(!_wiiSensor->readAcc(0,accPos)) return false;
		string newTxt = "";
		if(strcmp(labelName,"accX")==0){
			newTxt = utils::stringify(accPos.data()[0]);
		}else if(strcmp(labelName,"accY")==0){
			newTxt = utils::stringify(accPos.data()[1]);
		}else if(strcmp(labelName,"accZ")==0){
			newTxt = utils::stringify(accPos.data()[2]);
		}else if(strcmp(labelName,"accRoll")==0){
			newTxt = utils::stringify(accPos.data()[3]);
		}else if(strcmp(labelName,"accPitch")==0){
			newTxt = utils::stringify(accPos.data()[4]);
		}else if(strcmp(labelName,"accYaw")==0){
			newTxt = utils::stringify(accPos.data()[5]);
		}	
		gtk_label_set_text(label,newTxt.c_str());
		return TRUE;
	}
	if(strncmp(labelName,"mp",2)==0){
		Vector mpCoord, mpDiffCoord, mpAngle;
		mpCoord.resize(3);
		mpDiffCoord.resize(3);
		mpAngle.resize(3);
		if(!_wiiSensor->readMotionPlus(0,mpCoord))return true;
		if(!_wiiSensor->readDiffMotionPlus(0,mpDiffCoord))return true;
		if(!_wiiSensor->readMotionPlusAngle(0,mpAngle))return true;
		string newTxt = "";
		if(strcmp(labelName,"mpX")==0){
			newTxt = utils::stringify(mpCoord.data()[0]);
		}else if(strcmp(labelName,"mpY")==0){
			newTxt = utils::stringify(mpCoord.data()[1]);
		}else if(strcmp(labelName,"mpZ")==0){
			newTxt = utils::stringify(mpCoord.data()[2]);
		}else if(strcmp(labelName,"mpDiffX")==0){
			newTxt = utils::stringify(mpDiffCoord.data()[0]);
		}else if(strcmp(labelName,"mpDiffY")==0){
			newTxt = utils::stringify(mpDiffCoord.data()[1]);
		}else if(strcmp(labelName,"mpDiffZ")==0){
			newTxt = utils::stringify(mpDiffCoord.data()[2]);
		}else if(strcmp(labelName,"mpPitch")==0){
			newTxt = utils::stringify(mpAngle.data()[0]);
		}else if(strcmp(labelName,"mpRoll")==0){
			newTxt = utils::stringify(mpAngle.data()[1]);
		}else if(strcmp(labelName,"mpYaw")==0){
			newTxt = utils::stringify(mpAngle.data()[2]);
		}
		gtk_label_set_text(label,newTxt.c_str());
		return TRUE;
	}
	return FALSE;
}

GObject *Gui::getGObj(string gobjName){
	GObject *gObj = NULL;
	if(_builder != NULL)
		gObj = gtk_builder_get_object( _builder, gobjName.c_str() );
	return gObj;
}

GtkEntry *Gui::getEntry(string entryName){
	GtkEntry *entry =  GTK_ENTRY( getGObj(entryName) );
	return entry;
}

GtkLabel *Gui::getLabel(string labelName){
	GtkLabel *label =  GTK_LABEL( getGObj(labelName) );
	return label;
}

void Gui::setWidgetColor(GtkWidget *widget, string colorName){
	GdkColor *color = new GdkColor();
	gdk_color_parse (colorName.c_str(), color);
	gtk_widget_modify_base(widget,GTK_STATE_NORMAL,color);
}
