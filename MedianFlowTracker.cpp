/*
 * This file is part of OpenTLD.
 *
 * OpenTLD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenTLD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>

#include "MedianFlowTracker.h"

#include <cmath>

#include "FBTrack.h"

using namespace cv;
using namespace std;

namespace tld
{

MedianFlowTracker::MedianFlowTracker() {
	trackerBB = NULL;
}

MedianFlowTracker::~MedianFlowTracker() {
	cleanPreviousData();
}

void MedianFlowTracker::cleanPreviousData() {
	delete trackerBB;
	trackerBB = NULL;
}

void MedianFlowTracker::track(const Mat &prevMat, const Mat &currMat, Rect *prevBB, bool showResult) {
	if(prevBB != NULL) {	
		cout << "Median Flow Tracker Tracking...\n";
		if(prevBB->width <= 0 || prevBB->height <= 0) {
			return;
		}
	
		float bb_tracker[] = {prevBB->x, prevBB->y, prevBB->width + prevBB->x - 1, prevBB->height + prevBB->y - 1};
		float scale;
		
		IplImage prevImg = prevMat; // God, I really don't like C-style...
		IplImage currImg = currMat;
	
		int success = fbtrack(&prevImg, &currImg, bb_tracker, bb_tracker, &scale, showResult);
		
		//Extract subimage
		float x, y, w, h;
		x = floor(bb_tracker[0] + 0.5);
		y = floor(bb_tracker[1] + 0.5);
		w = floor(bb_tracker[2] - bb_tracker[0] + 1 + 0.5);
		h = floor(bb_tracker[3] - bb_tracker[1] + 1 + 0.5);

		//TODO: Introduce a check for a minimum size
		if(!success 
		|| x < 0 || y < 0 || w <= 0 || h <= 0 
		|| x + w > currMat.cols || y + h > currMat.rows ) {
	
		} else {
			trackerBB = new Rect(x, y, w, h);
		}
		
		cout << "Tracking Done!\n";
	}
}

} /* namespace tld */


