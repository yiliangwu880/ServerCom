#include "serialcode_parser.h"
#include "utility/logFile.h"


uint64_t RedeemCodeMgr::CodeToUint64(const std::string &code)
{
	uint64_t src = 0;
	for (uint32_t i = 0; i < RedeemCodeCfg::REDEEM_CODE_LEN; ++i)
	{
		uint64_t originNum = ConvertBase32CharToNum(code[i]);
		if (originNum >= 32)
			return false;  //字符非法

		src += (originNum << ((RedeemCodeCfg::REDEEM_CODE_BIT_LEN - 5) - i * 5));
	}
	return src;
}



bool RedeemCodeMgr::CheckCodeValid(const std::string &code)
{
	if (code.size() != RedeemCodeCfg::REDEEM_CODE_LEN)
		return false;
	uint64_t src = CodeToUint64(code);

	uint64_t sign_src = src >> RedeemCodeCfg::SIGN_BIT_LEN;
	uint32_t sign = src & RedeemCodeCfg::SIGN_MAX;

	uint32_t check_sign = GetSign(sign_src);
	if (sign != check_sign)
		return false;

	return true;
}


uint64_t RedeemCodeMgr::ConvertBase32CharToNum(char c)
{
	switch(c)
	{
		case 'A':
			return 0;
			break;
		case 'B':
			return 1;
			break;
		case 'C':
			return 2;
			break;
		case 'D':
			return 3;
			break;
		case 'E':
			return 4;
			break;
		case 'F':
			return 5;
			break;
		case 'G':
			return 6;
			break;
		case 'H':
			return 7;
			break;
		case 'J':
			return 8;
			break;
		case 'K':
			return 9;
			break;
		case 'L':
			return 10;
			break;
		case 'M':
			return 11;
			break;
		case 'N':
			return 12;
			break;
		case 'P':
			return 13;
			break;
		case 'Q':
			return 14;
			break;
		case 'R':
			return 15;
			break;
		case 'S':
			return 16;
			break;
		case 'T':
			return 17;
			break;
		case 'U':
			return 18;
			break;
		case 'V':
			return 19;
			break;
		case 'W':
			return 20;
			break;
		case 'X':
			return 21;
			break;
		case 'Y':
			return 22;
			break;
		case 'Z':
			return 23;
			break;

		case '2':
			return 24;
			break;
		case '3':
			return 25;
			break;
		case '4':
			return 26;
			break;
		case '5':
			return 27;
			break;
		case '6':
			return 28;
			break;
		case '7':
			return 29;
			break;
		case '8':
			return 30;
			break;
		case '9':
			return 31;
			break;

		default:
			return 999;
			break;
	}
}




std::string RedeemCodeMgr::CreateRedeemCode(uint32_t fun_id, uint32_t unique_code)
{
	if (fun_id > RedeemCodeCfg::FUN_ID_MAX)
	{
		LOG_ERROR("fun_id > RedeemCodeCfg::FUN_ID_MAX");
		return "";
	}
	if (unique_code > RedeemCodeCfg::UNIQUE_CODE_MAX)
	{
		LOG_ERROR("unique_code > RedeemCodeCfg::UNIQUE_CODE_MAX");
		return "";
	}

	const uint32_t UNIQUE_CODE_XOR_KEY[4] = { 0x7f8a4233, 0xba563e73, 0xaffa32ce, 0x5f8a3dd2 };		//顺序唯一码 异或加密用的 KEY，代码里面保密

	unique_code = unique_code ^ UNIQUE_CODE_XOR_KEY[unique_code%4]; //让顺序唯一码，转变下值，结果看起来不顺序。
	unique_code = unique_code & RedeemCodeCfg::UNIQUE_CODE_MAX;

	//让功能码 看起来不一样
	fun_id = unique_code ^ fun_id;
	fun_id = fun_id & RedeemCodeCfg::FUN_ID_MAX;

	uint64_t fun_id_seg = (uint64_t)fun_id << RedeemCodeCfg::FUN_ID_BIT_START_IDX;
	uint64_t unique_code_seg = (uint64_t)unique_code << RedeemCodeCfg::UNIQUE_CODE_BIT_START_IDX;
	uint64_t sign_src_seg = fun_id_seg | unique_code_seg;
	uint64_t sign_src = sign_src_seg >> RedeemCodeCfg::SIGN_BIT_LEN;
	uint32_t sign = GetSign(sign_src);

	uint64_t code = sign_src_seg | sign;

	return Uint64ToCode(code);
}

uint32_t RedeemCodeMgr::GetFunId(const std::string &code)
{
	uint64_t src = CodeToUint64(code);
	uint32_t unique_code = (src >> RedeemCodeCfg::UNIQUE_CODE_BIT_START_IDX)& RedeemCodeCfg::UNIQUE_CODE_MAX;
	uint32_t fun_id = (src >> RedeemCodeCfg::FUN_ID_BIT_START_IDX)& RedeemCodeCfg::FUN_ID_MAX;

	fun_id = unique_code ^ fun_id;
	fun_id = fun_id & RedeemCodeCfg::FUN_ID_MAX;
	return fun_id;
}

bool RedeemCodeMgr::CheckCfg()
{
	if (RedeemCodeCfg::SIGN_BIT_LEN * 2 > RedeemCodeCfg::REDEEM_CODE_BIT_LEN)
	{
		LOG_ERROR("RedeemCodeCfg::SIGN_BIT_LEN * 2 must <= RedeemCodeCfg::REDEEM_CODE_BIT_LEN");
		return false;
	}
	if (RedeemCodeCfg::REDEEM_CODE_BIT_LEN >64)
	{
		LOG_ERROR("RedeemCodeCfg::REDEEM_CODE_BIT_LEN >64");
		return false;
	}
	if (RedeemCodeCfg::SIGN_BIT_START_IDX != 0)
	{
		//签名开始位索引 必须0. 错了就是常量定义写错了。
		LOG_ERROR("RedeemCodeCfg::SIGN_BIT_START != 0"); 
		return false;
	}

	return true;
}

uint32_t RedeemCodeMgr::GetSign(uint32_t src)
{
	return (src ^ RedeemCodeCfg::SIGN_XOR_KEY) & RedeemCodeCfg::SIGN_MAX;
}

std::string RedeemCodeMgr::Uint64ToCode(uint64_t longRandValue)
{
	//45  9=45/5 , 5,0x1F 表示32进制
	std::string ret;

	const char *Base32Alphabet = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";

	for (uint32_t i = 0; i < RedeemCodeCfg::REDEEM_CODE_LEN; ++i)
	{
		//uint32_t idx = (tmpRandValue >> (40 - i * 5)) & (0x1F);
		uint32_t idx = (longRandValue >> ((RedeemCodeCfg::REDEEM_CODE_BIT_LEN - 5) - i * 5)) & (0x1F);
		ret.append(1, Base32Alphabet[idx]);
	}
	return ret;
}

RedeemCodeMgr::RedeemCodeMgr()
{
	if (!CheckCfg())
	{
		LOG_ERROR("cfg error");
	}
}

