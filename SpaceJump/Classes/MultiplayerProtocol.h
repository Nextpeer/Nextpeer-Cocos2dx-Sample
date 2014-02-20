//
//  Created by Nextpeer development team.
//  Copyright (c) 2014 Nextpeer, Inc. All rights reserved.
//

#ifndef __MultiplayerProtocol__
#define __MultiplayerProtocol__

#define PROTOCOL_VERSION 1

typedef struct __attribute__ ((__packed__)) {
    uint32_t protocolVersion;
    uint32_t messageType;
    
} MultiplayerMessageHeader;

typedef enum {
    MULTIPLAYER_MESSAGE_TYPE_UNKNOWN = 0,
    MULTIPLAYER_MESSAGE_TYPE_OPPONENT_POS_HEIGHT_UPDATE = 1,
    MULTIPLAYER_MESSAGE_TYPE_OPPONENT_IS_READY = 2
} MultiplayerMessageType;

#endif /* defined(__MultiplayerProtocol__) */