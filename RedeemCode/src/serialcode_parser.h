/*
游戏兑换码
功能流程： 工具 生成某个功能码n个，以后可以多次生成,不重复。需要文件保存历史记录。
码交给运营
客户端发出码，服务器验证签名通过, 验证码是否用过，根据功能发礼品，全局db插入码（表示领过）


加密功能id = 异或加密（功能id， key=功能唯一code）
兑换码= 加密功能id+功能唯一code+sign
唯一码=功能唯一code
sign = sign_fun(功能id+唯一码)  --要求 （功能id+唯一码）  的位数 >= sign位数
sign的最大值=  比如sign占二进制2位。 最大值为4。
假设所有功能id开放， 随机一个吗，随机出有效吗的概率 = 1/sign的最大值.


总结：sign位数越多，暴力破解越难.


实现细节
-------------------------------------------
)生成工具，历史存档，唯一码,用顺序序列号记录，记录信息小。  使用的时候，用序列号异或加密，生成看起来随机的唯一码.
)异或加密存放代码文件里。
)解决多个码，功能id 一样，导致前几个字符一样。  用唯一码异或功能id
*/


#pragma  once
#include <stdint.h>
#include <string>
#include <bitset>


struct RedeemCodeCfg
{
	static const uint32_t SIGN_XOR_KEY = 0x7f8a1231;			// 签名 异或加密用的 KEY，代码里面保密

	static const uint32_t REDEEM_CODE_LEN = 8;					//最终兑换码 长度

	static const uint32_t REDEEM_CODE_BIT_LEN = REDEEM_CODE_LEN * 5; //兑换码 位数,最大60 , 5的倍数。 因为要传32进制。 一位数最大0x7f, 5位

	//位段长度
	static const uint32_t SIGN_BIT_LEN = 10;						//sign位数 , 越长越安全,越安全，生成的码就越少
	static const uint32_t FUN_ID_BIT_LEN = 8;						//功能id位数
	static const uint32_t UNIQUE_CODE_BIT_LEN = REDEEM_CODE_BIT_LEN - SIGN_BIT_LEN - FUN_ID_BIT_LEN;		//唯一码id位数

	//位段开始索引
	static const uint32_t FUN_ID_BIT_START_IDX = REDEEM_CODE_BIT_LEN - FUN_ID_BIT_LEN;
	static const uint32_t UNIQUE_CODE_BIT_START_IDX = FUN_ID_BIT_START_IDX - UNIQUE_CODE_BIT_LEN;
	static const uint32_t SIGN_BIT_START_IDX = UNIQUE_CODE_BIT_START_IDX - SIGN_BIT_LEN;

	//每个段最大值
	static const uint32_t FUN_ID_MAX		= (0x1 << FUN_ID_BIT_LEN) - 1;		 //同时表示：兑换礼包功能数
	static const uint32_t UNIQUE_CODE_MAX	= (0x1 << UNIQUE_CODE_BIT_LEN) - 1;  //同时表示：每个功能最多多少个码
	static const uint32_t SIGN_MAX			= (0x1 << SIGN_BIT_LEN) - 1;		 //同时表示：暴力破解平均次数 = SIGN_MAX/2


};

//兑换码算法管理
class RedeemCodeMgr
{
	RedeemCodeMgr();
public:
	static RedeemCodeMgr &Instance()
	{
		static RedeemCodeMgr obj;
		return obj;
	}

	//根据 功能id+唯一码 生成兑换码, 唯一码可以按顺序传入，返回的码看不出顺序
	std::string CreateRedeemCode(uint32_t fun_id, uint32_t unique_code);
	//检查一个码是否合法
	bool CheckCodeValid(const std::string &code);
	uint32_t GetFunId(const std::string &code);


private:
	//return true表示配置 RedeemCodeCfg 定义ok
	bool CheckCfg();
	uint64_t CodeToUint64(const std::string &code);
	uint32_t GetSign(uint32_t src);
	uint64_t ConvertBase32CharToNum(char c);
	std::string Uint64ToCode(uint64_t src);
};

class RedeemCodeTool
{
	const char *CreateHistoryFile = "history.txt";
	RedeemCodeTool();
public:
	static RedeemCodeTool &Instance()
	{
		static RedeemCodeTool obj;
		return obj;
	}

	//生成兑换码
	//
	//return true 表示执行成功，false没任何改变.
	bool run(uint32_t fun_id, uint32_t create_num);


private:
   	

};