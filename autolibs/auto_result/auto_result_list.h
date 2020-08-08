#include "auto_result_maker.h"

ResultMakerStart()
ResultMaker( Success )
ResultMaker( Internal )
ResultMaker( InvalidParameter, "Invalid Parameter is passed." )
ResultMaker( MemoryAllocFail )
ResultMaker( Timeout )
ResultMakerEnd()

