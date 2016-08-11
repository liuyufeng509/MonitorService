#ifndef __NETMASTER_H__
#define __NETMASTER_H__

#ifdef WIN32
	//#ifdef DLL_EXPORTS
	//	#define DLLEXPORT __declspec(dllexport)
	//#else
	//	#define DLLEXPORT __declspec(dllimport)
	//#endif
	#define DLLEXPORT __declspec(dllexport)
#else
	#define DLLEXPORT  
#endif

#ifdef	__cplusplus
	extern "C" {
#endif

	//配置接收回调函数的定义
	typedef int (*ConfRecvCallBackFun)(
		const char *szConfType, const char *szConfXml, std::string &strAckXml);
	//参数const char *szConfType
	//		表示运维中心服务端下发配置信息的类型，用于区分下发信息中不同的请求或变更类型
	//参数const char *szConfXml
	//		表示运维中心服务端下发配置信息的内容
	//		包括检测端查询配置信息的返回内容，以及运维中心服务端主动下发的配置变更内容等
	//参数std::string &strAckXml
	//		可为空
	//		主要用于运维中心服务端主动下发的配置变更被检测端处理后，检测端返回配置变更的ACK内容

	/************************************************************************
	* Function      : SetRedisClusterAddrs
	* Description   : 网管前端设置Redis集群地址
	* Input         : const char *szAddrList	Redis集群地址（列表），格式为：[IP]:[Port]并以分号（;）分割
	* Output        : 无
	* Return        : 无
	* Note          :
	************************************************************************/
	void DLLEXPORT SetRedisClusterAddrs(const char *szAddrList);

	/************************************************************************
	* Function      : SetConfRecvCallBack
	* Description   : 网管前端设置配置接收回调
	* Input         : ConfRecvCallBackFun fCallBack	配置接收回调指针
	* Output        : 无
	* Return        : 无
	* Note          :
	************************************************************************/
	void DLLEXPORT SetConfRecvCallBack(ConfRecvCallBackFun fCallBack);

	/************************************************************************
	* Function      : NetMasterInit
	* Description   : 网管前端加载组件
	* Input         : const char *szAdress	网管后台服务端地址，格式为：[IP]:[Port]
					  const char *szUUID	检测端唯一标识，如调度单元的UUID等
					  int bitMark			特殊业务标志，如0x00表示无特殊业务、0x01表示需要建立通讯后反馈等
	* Output        : 无
	* Return        : 返回加载组件的结果，0表示加载成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterInit(const char *szAdress, const char *szUUID, unsigned char bitMark=0x00);

	/************************************************************************
	* Function      : NetMasterDestroy
	* Description   : 网管前端卸载组件
	* Input         : 无
	* Output        : 无
	* Return        : 返回卸载组件的结果，0表示卸载成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterDestroy();

	/************************************************************************
	* Function      : NetMasterReportData
	* Description   : 网管前端提报状态数据
	* Input         : const char * szStatusData		待提报的状态数据（XML格式）
	*				  const char *szAddrList		Redis集群地址（列表）
													格式为：[IP]:[Port]并以分号（;）分割
													参数可缺省
	* Output        : 无
	* Return        : 返回提报数据的结果，0表示提报成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterReportData(const char *szStatusData, const char *szAddrList=NULL);

	/************************************************************************
	* Function      : NetMasterPubNews
	* Description   : 网管前端发布信息
	* Input         : const char *szNews			待发布的信息（XML格式）
	*				  const char *szAddrList		Redis集群地址（列表）
													格式为：[IP]:[Port]并以分号（;）分割
													参数可缺省
	* Output        : 无
	* Return        : 返回发布信息的结果，0表示发布成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterPubNews(const char *szNews, const char *szAddrList=NULL);

	/************************************************************************
	* Function      : NetMasterPostData
	* Description   : 网管前端推送状态数据
	* Input         : const char * szStatusData		待推送的状态数据（XML格式）
	* Output        : 无
	* Return        : 返回推送数据的结果，0表示推送成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterPostData(const char *szStatusData);

	/************************************************************************
	* Function      : NetMasterQueryInfo
	* Description   : 网管前端获取配置信息
	* Input         : const char * szQueryType		查询信息的类型
					  const char * szQueryMsg		查询信息的内容(XML格式)
	* Output        : 无
	* Return        : 返回获取配置的结果，0表示获取成功，其他表示错误码
	* Note          :
	************************************************************************/
	int DLLEXPORT NetMasterQueryInfo(const char * szQueryType, const char *szQueryMsg);

	///************************************************************************
	//* Function      : NetMasterQueryInfo
	//* Description   : 网管前端获取配置信息
	//* Input         : const char * szQueryMsg		查询信息的请求(XML格式)
	//* Output        : std::string &strReturnInfo	返回的配置信息(XML格式)
	//* Return        : 返回获取配置的结果，0表示获取成功，其他表示错误码
	//* Note          :
	//************************************************************************/
	//int DLLEXPORT NetMasterQueryInfo(const char *szQueryMsg, std::string &strReturnInfo);

	/************************************************************************
	* Function      : NetManagerXml
	* Description   : 网管数据提交（原网管系统）
	* Input         : const char * szUrl     服务器地址(地址，端口号)
	*               : const char * szXmlData 提交数据(XML格式)
	* Output        : 
	* Return        : 0表示提交成功，其他表示提交失败
	* Note          :
	************************************************************************/
	int DLLEXPORT NetManagerXml(const char * szUrl, const char * szXmlData);
	
	/************************************************************************
	* Function      : QueryCameraXml
	* Description   : 数字矩阵单元查询摄像机状态函数（原网管系统）
	* Input         : const char * szUrl     服务器地址(地址，端口号)
	*               : const char * szXmlData 提交数据(XML格式)
	* Output        : nRusult，返回状态或错误代码
	* Return        : 0：成功，1,失败
	* Note          :
	************************************************************************/
	int DLLEXPORT QueryCameraXml(const char * szUrl, const char * szXmlData, int &nResult);

	/************************************************************************
	* Function      : LoadConfig
	* Description   : 配置下载（原网管系统）
	* Input         : const char * szUrl     服务器地址(地址，端口号)
	*               : const char * szXmlData 提交数据(XML格式)
	* Output        : 配置下载的数据
	* Return        : 0：成功，1,失败
	* Note          :
	************************************************************************/
	int DLLEXPORT LoadConfig(const char * szUrl, const char * szXmlData, std::string &strConfig);

#ifdef	__cplusplus
	};
#endif

#endif //__NETMASTER_H__
