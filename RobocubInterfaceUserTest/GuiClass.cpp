#include "GuiClass.h"

map<char*,widgetData*> GuiClass::_widgetMap;

GuiClass::GuiClass(char *filename)
{
	_builder = NULL;
	char *guiFilename = filename;
	//start the Gui
	init(guiFilename);
}

GuiClass::~GuiClass(void)
{
	/* Destroy builder */
	g_object_unref( G_OBJECT( _builder ) );
}

void GuiClass::init(char *guiFilename){
	/*Initializes GTK+*/
	gtk_init( NULL, NULL);
	/*Loads Gui from the XML file created by Glade*/
	loadGuiFile(guiFilename);
	/*Prepares the widgets with the important data and states*/
	setGuiWidgets();
	/*Connects callbacks to gui, for the buttons to work*/
	connectCallbacks();
	/*Adds idle/timeout funtions calls to the gui*/
	addIdleFunctions();
	/*Set gui as ON*/
	Util::guiData->isGuiON = true;
	/*Show Main Window and block in main loop*/
	showMainWindowAndBlock();
}

void GuiClass::loadGuiFile(char *guiFilename){
	GtkBuilder *builder = _builder = gtk_builder_new();
	int builderOK = gtk_builder_add_from_file( builder, guiFilename, NULL );
	if(builderOK) 
		printf("loadGuiFile():Success!\n");
	else 
		printf("loadGuiFile():FAILED!\n");
}

void GuiClass::connectCallbacks(){
	widgetData *data = NULL;
	GtkBuilder *builder = _builder;
	gtk_builder_connect_signals( builder, NULL );

	data = getNewWidgetData(builder,"startTestBot");
	g_signal_connect(gtk_builder_get_object( builder, data->name), "clicked", G_CALLBACK(button_clicked), data);
	data = getNewWidgetData(builder,"togglebuttonMotionPlus");
	g_signal_connect(gtk_builder_get_object( builder, data->name), "clicked", G_CALLBACK(button_clicked), data);
	data = getNewWidgetData(builder,"buttonRebootWii");
	_widgetMap["buttonRebootWii"] = data;
	g_signal_connect(gtk_builder_get_object( builder, data->name), "clicked", G_CALLBACK(button_clicked), data);
	data = getNewWidgetData(builder,"comboboxTest");
	g_signal_connect( gtk_builder_get_object( builder, data->name), "changed", G_CALLBACK( comboBox_Change ), data );
}

void GuiClass::setGuiWidgets(){
	GtkBuilder *builder = _builder;

	GtkWidget *comboBoxWidget = GTK_WIDGET(gtk_builder_get_object(builder,"comboboxTest"));
	char *comboBoxData[] = {"TesteA","TesteB","TesteC"};
	setComboBoxItems(GTK_COMBO_BOX(comboBoxWidget), 3, comboBoxData);
}

void GuiClass::addIdleFunctions(){
	GtkBuilder *builder = _builder;
	widgetData *data = NULL;

	data = getNewWidgetData(builder,"labelWiiState");
	g_idle_add((GSourceFunc)GuiClass::idleFuncLAbel, data);
	data = getNewWidgetData(builder,"labelICubState");
	g_idle_add((GSourceFunc)GuiClass::idleFuncLAbel, data);
	data = getNewWidgetData(builder,"labelMP");
	g_idle_add((GSourceFunc)GuiClass::idleFuncLAbel, data);
	data = getNewWidgetData(builder,"labelAcc");
	g_idle_add((GSourceFunc)GuiClass::idleFuncLAbel, data);
	data = getNewWidgetData(builder,"labelAngle");
	g_idle_add((GSourceFunc)GuiClass::idleFuncLAbel, data);
	data = getNewWidgetData(builder,"progressbarBattery");
	g_idle_add((GSourceFunc)GuiClass::idleFuncProgressBar, data);
	data = getNewWidgetData(builder,"drawingareaMP");
	g_idle_add((GSourceFunc)GuiClass::setDrawingArea, data);
}

void GuiClass::showMainWindowAndBlock(){
	GtkWidget  *window;
	GtkBuilder *builder = _builder;

	/* Obtain widgets that we need */
	window = GTK_WIDGET( gtk_builder_get_object( builder, "mainWindow" ) );
	/* Destroy builder */
	g_object_unref( G_OBJECT( builder ) );
	_builder = NULL;
	/* Show main window and start main loop */
	gtk_widget_show( window );
	gtk_main();
}

/*GTK BUTTON FUNCTIONS*/
void GuiClass::button_clicked(GtkWidget *widget, gpointer callData){
	widgetData *data = static_cast<widgetData *>(callData);
	char *widgetName = data->name;
	/*startTestBot*/
	if(strcmp(widgetName,"startTestBot") == 0){
		GtkToggleButton *bot = GTK_TOGGLE_BUTTON(widget);
		if(bot->active) //if bot active lock all entry widgets
			gtk_label_set_text(GTK_LABEL(GTK_BIN(bot)->child), "STOP test!");
		else gtk_label_set_text(GTK_LABEL(GTK_BIN(bot)->child), "Start test!");
	}
	/*startTestBot*/
	if(strcmp(widgetName,"togglebuttonMotionPlus") == 0){
		GtkToggleButton *bot = GTK_TOGGLE_BUTTON(widget);
		if(bot->active)
			gtk_label_set_text(GTK_LABEL(GTK_BIN(bot)->child), "Accelerometer & MotionPlus ON!");
		else gtk_label_set_text(GTK_LABEL(GTK_BIN(bot)->child), "Accelerometer & MotionPlus OFF");
		Util::guiData->mpBot = bot->active;
	}
	if(strcmp(widgetName,"buttonRebootWii") == 0){
		//LOCKS!!!!!
		Util::guiData->wiiRebootBot = true;
		//GtkToggleButton *bot = GTK_TOGGLE_BUTTON(_widgetMap["buttonRebootWii"]->widget);
	}
}

void GuiClass::comboBox_Change(GtkComboBox *combo, gpointer callData){
	widgetData *data = static_cast<widgetData *>(callData);

	GtkTreeIter   iter;
	gchar        *string = NULL;
	GtkTreeModel *model;
	/* Obtain currently selected item from combo box.
	* If nothing is selected, do nothing. */
	if( gtk_combo_box_get_active_iter( combo, &iter ) )
	{
		/* Obtain data model from combo box. */
		model = gtk_combo_box_get_model( combo );
		/* Obtain string from model. */
		gtk_tree_model_get( model, &iter, 0, &string, -1 );
	}
	/* Print string to the console - if string is NULL, print NULL. */
	g_print( "Selected (complex): >> %s <<\n", ( string ? string : "NULL" ) );
	/* Free string (if not NULL). */
	if( string )
		g_free( string );
}

/*GTK IDLE FUNCTIONS*/
gboolean GuiClass::idleFuncLAbel(widgetData *widgetData){
	char buffer[100];

	if(strcmp(widgetData->name,"labelWiiState")==0){
		setLabelColoredFlag(widgetData->widget,Util::wiiData->isConnected/*Wii state*/,"Wii ON!","Wii OFF");
		return true;
	}
	if(strcmp(widgetData->name,"labelICubState")==0){
		setLabelColoredFlag(widgetData->widget,Util::icubData->isConnected/*iCub state*/,"iCub ON!","iCub OFF");
		return true;
	}
	if(strcmp(widgetData->name,"labelMP")==0){
		buffer[0] = '\0';
		char *speed = Util::wiiData->isMPLowSpeed==-1?"N/A":(Util::wiiData->isMPLowSpeed?" LOW":"HIGH");
		//sprintf_s(buffer, "%s(%0+7.2f, %0+7.2f, %0+7.2f)",speed, Util::wiiData->motionPlusX, Util::wiiData->motionPlusY, Util::wiiData->motionPlusZ);
		double angleX = Util::wiiData->motionPlusAngleX/DEG_TO_RAD;
		double angleY = Util::wiiData->motionPlusAngleY/DEG_TO_RAD;
		double angleZ = Util::wiiData->motionPlusAngleZ/DEG_TO_RAD;
		sprintf_s(buffer, "%s(%0+7.2f, %0+7.2f, %0+7.2f)",speed, angleX, angleY, angleZ);
		gtk_label_set_text(GTK_LABEL(widgetData->widget),buffer);
		return true;
	}
	if(strcmp(widgetData->name,"labelAcc")==0){
		buffer[0] = '\0';
		sprintf_s(buffer, "(%.2f, %.2f, %.2f)", Util::wiiData->accX, Util::wiiData->accY, Util::wiiData->accZ);
		gtk_label_set_text(GTK_LABEL(widgetData->widget),buffer);
		return true;
	}
	if(strcmp(widgetData->name,"labelAngle")==0){
		buffer[0] = '\0';
		sprintf_s(buffer, "(%.2f, %.2f, %.2f)", Util::wiiData->yaw, Util::wiiData->pitch, Util::wiiData->roll);
		gtk_label_set_text(GTK_LABEL(widgetData->widget),buffer);
		return true;
	}
	return true;
}

gboolean GuiClass::idleFuncProgressBar(widgetData *widgetData){
	char buffer[100];
	if(strcmp(widgetData->name,"progressbarBattery")==0){
		if(Util::wiiData->battery>=0){
			buffer[0] = '\0';
			sprintf_s(buffer, "%.0f%%", Util::wiiData->battery*100);
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgetData->widget), buffer);
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(widgetData->widget),Util::wiiData->battery);
		}else {
			gtk_progress_bar_set_text(GTK_PROGRESS_BAR(widgetData->widget), "-1");
			gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(widgetData->widget),0);
		}
		return true;
	}
	return true;
}

gboolean GuiClass::setDrawingArea(widgetData *widgetData)
{
	// set up colors and some GCs
	GtkDrawingArea *drawArea = GTK_DRAWING_AREA(widgetData->widget);
	GdkWindow	*window = drawArea->widget.window;
	int width = drawArea->widget.allocation.width;
	int height = drawArea->widget.allocation.height;
	int circleRadius = width<height?width:height;
	int lineThick = 2;
	GdkGC		*bg_gc = getGColor(window,65535,65535,65535);
	GdkGC		*green_gc = getGColor(window,0,65535,0);
	GdkGC		*blue_gc = getGColor(window,0,0,65535);
	GdkGC		*red_gc = getGColor(window,65535,0,0);
	GdkGC		*yellow_gc = getGColor(window,65535,65000,0);
	GdkGC		*black_gc = getGColor(window,0,0,0);

	gdk_draw_arc (window,bg_gc,TRUE, 0, 0, circleRadius, circleRadius,0, 360*64);

	double angleRadians = Util::wiiData->motionPlusAngleX;
	drawMotionPlusAngleLine(window, green_gc, angleRadians,circleRadius/2,lineThick);

	angleRadians = Util::wiiData->motionPlusAngleY;
	drawMotionPlusAngleLine(window, blue_gc, angleRadians,circleRadius/2,lineThick);

	angleRadians = Util::wiiData->motionPlusAngleZ;
	drawMotionPlusAngleLine(window, red_gc, angleRadians,circleRadius/2,lineThick);

	angleRadians = Util::wiiData->temp;
	drawMotionPlusAngleLine(window, yellow_gc, angleRadians,circleRadius/2,lineThick);

	angleRadians = Util::icubData->oriRightArm[3];
	drawMotionPlusAngleLine(window, black_gc, angleRadians,circleRadius/2,lineThick);

	gdk_draw_arc(window, black_gc, TRUE, circleRadius/2,circleRadius/2, lineThick, lineThick, 0, 360*64);

//printf("draw(%0+7.2f,%0+7.2f,%0+7.2f);\n",Util::wiiData->motionPlusAngleX,Util::wiiData->motionPlusAngleY,Util::wiiData->motionPlusAngleZ);
	return true;
}


/*GTK UTIL FUNCTIONS*/
void GuiClass::drawMotionPlusAngleLine(GdkWindow *window, GdkGC *color, double angleRadians, int lenghtofline, int lineThick){
	lenghtofline -= 1;
	int initX = (int)(sin(angleRadians)*lenghtofline)+lenghtofline;
	int initY = (int)(cos(angleRadians)*lenghtofline)+lenghtofline;
	int endX = (int)/*(sin(angleRadians + 3.14159265)*lenghtofline)+*/lenghtofline;
	int endY = (int)/*(cos(angleRadians + 3.14159265)*lenghtofline)+*/lenghtofline;
	gdk_gc_set_line_attributes(color,lineThick,GDK_LINE_SOLID,GDK_CAP_ROUND,GDK_JOIN_ROUND);
	gdk_draw_line (window, color, initX, initY, endX, endY);
}

void GuiClass::setComboBoxItems(GtkComboBox *comboBox, int arrayLength, char** stringArray){
	/* Create data store ... */
	GtkCellRenderer *cell;
	GtkTreeIter      iter;
	GtkListStore *store = gtk_list_store_new( 1, G_TYPE_STRING );

	/* ... and fill it with some information. */
	for(int i = 0; i < arrayLength; i++){
		gtk_list_store_insert( store, &iter, i );
		gtk_list_store_set( store, &iter, 0, stringArray[i], -1 );
	}
	/*add tree model to the ComboBox*/
	gtk_combo_box_set_model(GTK_COMBO_BOX(comboBox), GTK_TREE_MODEL(store));
	/*free memory*/
	g_object_unref( G_OBJECT( store ) );
	/* Create cell renderer. */
	cell = gtk_cell_renderer_text_new();
	/* Pack it into the combo box. */
	gtk_cell_layout_pack_start( GTK_CELL_LAYOUT( comboBox ), cell, TRUE );
	/* Connect renderer to data source. */
	gtk_cell_layout_set_attributes( GTK_CELL_LAYOUT( comboBox ), cell, "text", 0, NULL );
}

void GuiClass::setLabelColoredFlag(GtkWidget *widget, bool state, char *onString, char *offString){
	if(state){
		setWidgetColor(widget->parent,"green");
		gtk_label_set_text(GTK_LABEL(widget),onString);
	}
	else{
		setWidgetColor(widget->parent,"red");
		gtk_label_set_text(GTK_LABEL(widget),offString);
	}
}

void GuiClass::setWidgetColor(GtkWidget *widget, vector<int> colorVector){
	GdkColor *color = getGdkColor(colorVector[0],colorVector[1],colorVector[2]);
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,color);
}

void GuiClass::setWidgetColor(GtkWidget *widget, string colorName){
	GdkColor *color = new GdkColor();
	gdk_color_parse (colorName.c_str(), color);
	//gtk_widget_modify_base(widget,GTK_STATE_NORMAL,color);
	//gtk_widget_modify_fg(widget,GTK_STATE_NORMAL,color);
	gtk_widget_modify_bg(widget,GTK_STATE_NORMAL,color);
}

GdkGC *GuiClass::getGColor(GdkDrawable *drawable, int red, int green, int blue){
	GdkColor color;
	GdkGC *color_gc = gdk_gc_new(drawable);
	//0 to 65535
	color.red = red;
	color.green = green;
	color.blue = blue;
	gdk_gc_set_rgb_fg_color( color_gc, &color);
	return color_gc;
}

GdkColor *GuiClass::getGdkColor(int red, int green, int blue){
	GdkColor *color = new GdkColor();
	//0 to 65535
	color->red = red;
	color->green = green;
	color->blue = blue;
	return color;
}

widgetData *GuiClass::getNewWidgetData(GtkBuilder * builder, char *widgetName){
	widgetData *data = new widgetData();
	data->name = widgetName;
	data->widget = GTK_WIDGET(gtk_builder_get_object( builder, data->name));
	return data;
}
