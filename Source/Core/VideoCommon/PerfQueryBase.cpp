#include "VideoCommon/PerfQueryBase.h"
#include "VideoCommon/VideoConfig.h"

PerfQueryBase* g_perf_query = 0;

bool PerfQueryBase::ShouldEmulate() const
{
	return g_ActiveConfig.bPerfQueriesEnable;
}
