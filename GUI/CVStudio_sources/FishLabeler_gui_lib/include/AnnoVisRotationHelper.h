#pragma once 

#include "AnnoVisEmbryoBox.h"

struct ClosestKeyFrameOutput
{
	unsigned int leftKeyFrame;
	unsigned int leftEmbryoId;
	unsigned int rightKeyFrame;
	unsigned int rightEmbryoId;
	unsigned int centerEmbryoID;
	unsigned int centerKeyFrame;
	unsigned int embryoTrackID;
};

ClosestKeyFrameOutput findClosestKeyFrames(
	const std::vector<std::vector<EmbryoBox>>& imagesEmbryoBoxes,
	const unsigned int frameID,
	const unsigned int embryoId);

void setRotationsBetweenFoundKeyFrames(
	std::vector<std::vector<EmbryoBox>>& imagesEmbryoBoxes,
	const ClosestKeyFrameOutput& closestKeyFrameOutput
);

QVector3D shortestDistance(const QVector3D& from, const QVector3D& to);

int shortestDistanceAxis(const int from, const int to);

