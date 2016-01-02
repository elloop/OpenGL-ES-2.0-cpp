#define USING_NS_ELLOOP using namespace elloop
#define USING_NS_STD using namespace std
#define NS_ELLOOP_BEGIN namespace elloop {
#define NS_ELLOOP_END }
#define NS_BEGIN(x) namespace x {
#define NS_END(x) }

#ifdef _MSC_VER
#define __func__ __FUNCTION__
//#define noexcept _NOEXCEPT
#define _Pragma __pragma 
#endif

#define BREAK_IF(cond) if (cond) break;
