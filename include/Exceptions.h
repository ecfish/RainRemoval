/*
 * Created by zhanghan, 2019.10.11
 */

namespace VEXCEPTION{

    const int VEXCEPTION_BASE                     = 100;

    const int VIDEO_SRC_CAN_NOT_OPEN              = VEXCEPTION_BASE + 1;
    const int VIDEO_FRAME_INT_UNIT_UNKNOWN        = VEXCEPTION_BASE + 2;
    const int NONE_REST_VIDEO_FRAME               = VEXCEPTION_BASE + 3;

}

namespace SPEXCEPTION{

    const int SPEXCEPTION_BASE                    = 200;

    const int NS_TOO_BUG                          = SPEXCEPTION_BASE + 1;
    const int NS_NOT_ODD                          = SPEXCEPTION_BASE + 2;
    const int ST_BUFFER_INDEX_OUT_OF_RANGE        = SPEXCEPTION_BASE + 3;

}

namespace STBEXCEPTION{

    const int STBEXCEPTION_BASE                   = 300;

    const int ST_BUFFER_INDEX_OUT_OF_RANGE        = STBEXCEPTION_BASE + 1;
}