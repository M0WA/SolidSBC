#pragma once

#ifdef SOLIDSBCNETLIB_EXPORTS
#define SOLIDSBCNETLIB_API __declspec(dllexport)
#else
#define SOLIDSBCNETLIB_API __declspec(dllimport)
#endif

#include "SolidSBCPacket.h"

//config packets
#include "SolidSBCPacketConfigRequest.h"
#include "SolidSBCPacketConfigResponse.h"

//result packets
#include "SolidSBCPacketResultRequest.h"
#include "SolidSBCPacketTestResult.h"

//socket management
#include "SolidSBCSocket.h"
#include "SolidSBCSocketServer.h"
#include "SolidSBCSocketClient.h"