#pragma once

#include <time.h>
#include <XnCppWrapper.h>
#include <XnVPointControl.h>

using namespace xn;

typedef enum
{
	IN_SESSION,
	NOT_IN_SESSION,
	QUICK_REFOCUS
} SessionState;

class PointTracker : public XnVPointControl
{
public:
	typedef struct POINT_TRACKER_STATUS {
		long pointLastUpdate;
		double pointProjCoords[3];
		double pointRealCoords[3];
	}PointTrackerStatus;
	PointTracker(DepthGenerator *depthGen);
	~PointTracker(void);

	/**
	 * Handle a new message.
	 * Calls other callbacks for each point, then draw the depth map (if needed) and the points
	 */
	void Update(XnVMessage* pMessage);

	/**
	 * Handle creation of a new point
	 */
	void OnPointCreate(const XnVHandPointContext* cxt);
	/**
	 * Handle new position of an existing point
	 */
	void OnPointUpdate(const XnVHandPointContext* cxt);
	/**
	 * Handle destruction of an existing point
	 */
	void OnPointDestroy(XnUInt32 nID);
	PointTrackerStatus *getPointTracker();
private:
	DepthGenerator *_depthGen;
	PointTrackerStatus *_pointTrackerStatus;
	void initPointTrackerStatus();
};
