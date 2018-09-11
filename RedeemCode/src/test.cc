#include "stdio.h"
#include "utility/logFile.h"
#include "serialcode_parser.h"

using namespace std;

struct FunId_Uin
{
	uint32_t fun_id, uin_code;
};

void test()
{
	{//主流程
		FunId_Uin ar[] = {
			{ 1, 1 },
			{ 1, 2 },
			{ 1, 3 },
			{ 1, 4 },
			{ 1, 5 },
			{ 1, 6 },
			{ 134, 1 },
			{ 134, 2 },
			{ 134, 3004 },
		};
		for (FunId_Uin v : ar)
		{
			string code = RedeemCodeMgr::Instance().CreateRedeemCode(v.fun_id, v.uin_code);
			LOG_DEBUG("fun_id,uin_code,CreateRedeemCode==%d,%d,%s\n", v.fun_id, v.uin_code, code.c_str());
			ASSERT_DEBUG(RedeemCodeMgr::Instance().GetFunId(code) == v.fun_id);
			ASSERT_DEBUG(RedeemCodeMgr::Instance().CheckCodeValid(code));
		}
	}
	{
	ASSERT_DEBUG(!RedeemCodeMgr::Instance().CheckCodeValid("C2TZA68G"));//随机乱破解，出现正确的概率非常低

	LOG_DEBUG("\n\n----------------------------------follow error log is ok. but can't assert error----------------------------------");
	ASSERT_DEBUG("" != RedeemCodeMgr::Instance().CreateRedeemCode(RedeemCodeCfg::FUN_ID_MAX, 1));
	//fun_id超出范围
	ASSERT_DEBUG("" == RedeemCodeMgr::Instance().CreateRedeemCode(RedeemCodeCfg::FUN_ID_MAX + 1, 1));
	ASSERT_DEBUG("" == RedeemCodeMgr::Instance().CreateRedeemCode(RedeemCodeCfg::FUN_ID_MAX, RedeemCodeCfg::UNIQUE_CODE_MAX+1));
	//RedeemCode::Instance().
	}
}