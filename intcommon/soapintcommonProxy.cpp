/* soapintcommonProxy.cpp
   Generated by gSOAP 2.8.30 for BaseInterface.h

gSOAP XML Web services tools
Copyright (C) 2000-2016, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#include "soapintcommonProxy.h"

intcommonProxy::intcommonProxy() : soap(SOAP_IO_DEFAULT)
{	intcommonProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

intcommonProxy::intcommonProxy(const intcommonProxy& rhs)
{	soap_copy_context(this, &rhs);
	this->soap_endpoint = rhs.soap_endpoint;
}

intcommonProxy::intcommonProxy(const struct soap &_soap) : soap(_soap)
{ }

intcommonProxy::intcommonProxy(const char *endpoint) : soap(SOAP_IO_DEFAULT)
{	intcommonProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
	soap_endpoint = endpoint;
}

intcommonProxy::intcommonProxy(soap_mode iomode) : soap(iomode)
{	intcommonProxy_init(iomode, iomode);
}

intcommonProxy::intcommonProxy(const char *endpoint, soap_mode iomode) : soap(iomode)
{	intcommonProxy_init(iomode, iomode);
	soap_endpoint = endpoint;
}

intcommonProxy::intcommonProxy(soap_mode imode, soap_mode omode) : soap(imode, omode)
{	intcommonProxy_init(imode, omode);
}

intcommonProxy::~intcommonProxy()
{
	this->destroy();
	}

void intcommonProxy::intcommonProxy_init(soap_mode imode, soap_mode omode)
{	soap_imode(this, imode);
	soap_omode(this, omode);
	soap_endpoint = NULL;
	static const struct Namespace namespaces[] = {
        {"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
        {"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
        {"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
        {"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
        {"ns", "http://haibogroup.com.cn", NULL, NULL},
        {"ns1", "http://servlet.manage.haibo.com", NULL, NULL},
        {"nsCXF", "http://servlet.webservice.skynet.botech.com/", NULL, NULL},
        {NULL, NULL, NULL, NULL}
    };
	soap_set_namespaces(this, namespaces);
}

#ifndef WITH_PURE_VIRTUAL
intcommonProxy *intcommonProxy::copy()
{	intcommonProxy *dup = SOAP_NEW_COPY(intcommonProxy(*(struct soap*)this));
	return dup;
}
#endif

intcommonProxy& intcommonProxy::operator=(const intcommonProxy& rhs)
{	soap_copy_context(this, &rhs);
	this->soap_endpoint = rhs.soap_endpoint;
	return *this;
}

void intcommonProxy::destroy()
{	soap_destroy(this);
	soap_end(this);
}

void intcommonProxy::reset()
{	this->destroy();
	soap_done(this);
	soap_initialize(this);
	intcommonProxy_init(SOAP_IO_DEFAULT, SOAP_IO_DEFAULT);
}

void intcommonProxy::soap_noheader()
{	this->header = NULL;
}

::SOAP_ENV__Header *intcommonProxy::soap_header()
{	return this->header;
}

::SOAP_ENV__Fault *intcommonProxy::soap_fault()
{	return this->fault;
}

const char *intcommonProxy::soap_fault_string()
{	return *soap_faultstring(this);
}

const char *intcommonProxy::soap_fault_detail()
{	return *soap_faultdetail(this);
}

int intcommonProxy::soap_close_socket()
{	return soap_closesock(this);
}

int intcommonProxy::soap_force_close_socket()
{	return soap_force_closesock(this);
}

void intcommonProxy::soap_print_fault(FILE *fd)
{	::soap_print_fault(this, fd);
}

#ifndef WITH_LEAN
#ifndef WITH_COMPAT
void intcommonProxy::soap_stream_fault(std::ostream& os)
{	::soap_stream_fault(this, os);
}
#endif

char *intcommonProxy::soap_sprint_fault(char *buf, size_t len)
{	return ::soap_sprint_fault(this, buf, len);
}
#endif

int intcommonProxy::SwitchCameraToStream(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, struct ns__HBNET_VIDEO_STREAM *Url)
{	struct soap *soap = this;
	struct ns__SwitchCameraToStream soap_tmp_ns__SwitchCameraToStream;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__SwitchCameraToStream.nsUserInfo = nsUserInfo;
	soap_tmp_ns__SwitchCameraToStream.CamID = CamID;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__SwitchCameraToStream(soap, &soap_tmp_ns__SwitchCameraToStream);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__SwitchCameraToStream(soap, &soap_tmp_ns__SwitchCameraToStream, "ns:SwitchCameraToStream", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__SwitchCameraToStream(soap, &soap_tmp_ns__SwitchCameraToStream, "ns:SwitchCameraToStream", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Url)
		return soap_closesock(soap);
	soap_default_ns__HBNET_VIDEO_STREAM(soap, Url);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__HBNET_VIDEO_STREAM(soap, Url, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::PTZCtrlCamera(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, struct ns__HBNET_CAMERA_PTZCTRL *Data, struct ns__Response *Result)
{	struct soap *soap = this;
	struct ns__PTZCtrlCamera soap_tmp_ns__PTZCtrlCamera;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__PTZCtrlCamera.nsUserInfo = nsUserInfo;
	soap_tmp_ns__PTZCtrlCamera.CamID = CamID;
	soap_tmp_ns__PTZCtrlCamera.Data = Data;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__PTZCtrlCamera(soap, &soap_tmp_ns__PTZCtrlCamera);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__PTZCtrlCamera(soap, &soap_tmp_ns__PTZCtrlCamera, "ns:PTZCtrlCamera", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__PTZCtrlCamera(soap, &soap_tmp_ns__PTZCtrlCamera, "ns:PTZCtrlCamera", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Result)
		return soap_closesock(soap);
	soap_default_ns__Response(soap, Result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__Response(soap, Result, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::AbandonCameraStream(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, struct ns__Response *Result)
{	struct soap *soap = this;
	struct ns__AbandonCameraStream soap_tmp_ns__AbandonCameraStream;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__AbandonCameraStream.nsUserInfo = nsUserInfo;
	soap_tmp_ns__AbandonCameraStream.CamID = CamID;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__AbandonCameraStream(soap, &soap_tmp_ns__AbandonCameraStream);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__AbandonCameraStream(soap, &soap_tmp_ns__AbandonCameraStream, "ns:AbandonCameraStream", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__AbandonCameraStream(soap, &soap_tmp_ns__AbandonCameraStream, "ns:AbandonCameraStream", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Result)
		return soap_closesock(soap);
	soap_default_ns__Response(soap, Result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__Response(soap, Result, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::QueryVideoRecord(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, struct ns__HBNET_RECOED_FILECOND *Param, char *&RecodeList)
{	struct soap *soap = this;
	struct ns__QueryVideoRecord soap_tmp_ns__QueryVideoRecord;
	struct ns__QueryVideoRecordResponse *soap_tmp_ns__QueryVideoRecordResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__QueryVideoRecord.nsUserInfo = nsUserInfo;
	soap_tmp_ns__QueryVideoRecord.CamID = CamID;
	soap_tmp_ns__QueryVideoRecord.Param = Param;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__QueryVideoRecord(soap, &soap_tmp_ns__QueryVideoRecord);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__QueryVideoRecord(soap, &soap_tmp_ns__QueryVideoRecord, "ns:QueryVideoRecord", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__QueryVideoRecord(soap, &soap_tmp_ns__QueryVideoRecord, "ns:QueryVideoRecord", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	RecodeList = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__QueryVideoRecordResponse = soap_get_ns__QueryVideoRecordResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__QueryVideoRecordResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	RecodeList = soap_tmp_ns__QueryVideoRecordResponse->RecodeList;
	return soap_closesock(soap);
}

int intcommonProxy::PlayVideoRecord(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, struct ns__HBNET_VIDEO_STREAM *Data, struct ns__HBNET_VIDEO_STREAM_RESP *Out)
{	struct soap *soap = this;
	struct ns__PlayVideoRecord soap_tmp_ns__PlayVideoRecord;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__PlayVideoRecord.nsUserInfo = nsUserInfo;
	soap_tmp_ns__PlayVideoRecord.CamID = CamID;
	soap_tmp_ns__PlayVideoRecord.Data = Data;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__PlayVideoRecord(soap, &soap_tmp_ns__PlayVideoRecord);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__PlayVideoRecord(soap, &soap_tmp_ns__PlayVideoRecord, "ns:PlayVideoRecord", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__PlayVideoRecord(soap, &soap_tmp_ns__PlayVideoRecord, "ns:PlayVideoRecord", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Out)
		return soap_closesock(soap);
	soap_default_ns__HBNET_VIDEO_STREAM_RESP(soap, Out);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__HBNET_VIDEO_STREAM_RESP(soap, Out, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::DownloadVideoRecord(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, char *szFileName, char *&OutFileName)
{	struct soap *soap = this;
	struct ns__DownloadVideoRecord soap_tmp_ns__DownloadVideoRecord;
	struct ns__DownloadVideoRecordResponse *soap_tmp_ns__DownloadVideoRecordResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__DownloadVideoRecord.nsUserInfo = nsUserInfo;
	soap_tmp_ns__DownloadVideoRecord.CamID = CamID;
	soap_tmp_ns__DownloadVideoRecord.szFileName = szFileName;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__DownloadVideoRecord(soap, &soap_tmp_ns__DownloadVideoRecord);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__DownloadVideoRecord(soap, &soap_tmp_ns__DownloadVideoRecord, "ns:DownloadVideoRecord", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__DownloadVideoRecord(soap, &soap_tmp_ns__DownloadVideoRecord, "ns:DownloadVideoRecord", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	OutFileName = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__DownloadVideoRecordResponse = soap_get_ns__DownloadVideoRecordResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__DownloadVideoRecordResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	OutFileName = soap_tmp_ns__DownloadVideoRecordResponse->OutFileName;
	return soap_closesock(soap);
}

int intcommonProxy::AbandonVideoRecord(const char *endpoint, const char *soap_action, struct ns__HBUSERINFO nsUserInfo, long CamID, char *FileName, struct ns__Response *Result)
{	struct soap *soap = this;
	struct ns__AbandonVideoRecord soap_tmp_ns__AbandonVideoRecord;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__AbandonVideoRecord.nsUserInfo = nsUserInfo;
	soap_tmp_ns__AbandonVideoRecord.CamID = CamID;
	soap_tmp_ns__AbandonVideoRecord.FileName = FileName;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__AbandonVideoRecord(soap, &soap_tmp_ns__AbandonVideoRecord);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__AbandonVideoRecord(soap, &soap_tmp_ns__AbandonVideoRecord, "ns:AbandonVideoRecord", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__AbandonVideoRecord(soap, &soap_tmp_ns__AbandonVideoRecord, "ns:AbandonVideoRecord", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Result)
		return soap_closesock(soap);
	soap_default_ns__Response(soap, Result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__Response(soap, Result, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::HeartBeat(const char *endpoint, const char *soap_action, struct ns_HeartBeat nsHeartBeat, struct ns__Response *Result)
{	struct soap *soap = this;
	struct ns__HeartBeat soap_tmp_ns__HeartBeat;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__HeartBeat.nsHeartBeat = nsHeartBeat;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__HeartBeat(soap, &soap_tmp_ns__HeartBeat);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__HeartBeat(soap, &soap_tmp_ns__HeartBeat, "ns:HeartBeat", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__HeartBeat(soap, &soap_tmp_ns__HeartBeat, "ns:HeartBeat", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Result)
		return soap_closesock(soap);
	soap_default_ns__Response(soap, Result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__Response(soap, Result, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::AutoSysUpdate(const char *endpoint, const char *soap_action, std::string strXmlFile, struct ns__Response *Result)
{	struct soap *soap = this;
	struct ns__AutoSysUpdate soap_tmp_ns__AutoSysUpdate;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__AutoSysUpdate.strXmlFile = strXmlFile;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__AutoSysUpdate(soap, &soap_tmp_ns__AutoSysUpdate);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__AutoSysUpdate(soap, &soap_tmp_ns__AutoSysUpdate, "ns:AutoSysUpdate", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__AutoSysUpdate(soap, &soap_tmp_ns__AutoSysUpdate, "ns:AutoSysUpdate", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!Result)
		return soap_closesock(soap);
	soap_default_ns__Response(soap, Result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_get_ns__Response(soap, Result, "", NULL);
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

int intcommonProxy::NetMaster(const char *endpoint, const char *soap_action, int nIsOK, int &nResult)
{	struct soap *soap = this;
	struct ns__NetMaster soap_tmp_ns__NetMaster;
	struct ns__NetMasterResponse *soap_tmp_ns__NetMasterResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__NetMaster.nIsOK = nIsOK;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__NetMaster(soap, &soap_tmp_ns__NetMaster);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__NetMaster(soap, &soap_tmp_ns__NetMaster, "ns:NetMaster", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__NetMaster(soap, &soap_tmp_ns__NetMaster, "ns:NetMaster", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!&nResult)
		return soap_closesock(soap);
	soap_default_int(soap, &nResult);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__NetMasterResponse = soap_get_ns__NetMasterResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__NetMasterResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	nResult = soap_tmp_ns__NetMasterResponse->nResult;
	return soap_closesock(soap);
}

int intcommonProxy::HBOperate(const char *endpoint, const char *soap_action, char *szXml, char *&szXmlRes)
{	struct soap *soap = this;
	struct ns__HBOperate soap_tmp_ns__HBOperate;
	struct ns__HBOperateResponse *soap_tmp_ns__HBOperateResponse;
	if (endpoint)
		soap_endpoint = endpoint;
	if (soap_endpoint == NULL)
		soap_endpoint = "http://127.0.0.1:8086/live.fcgi";
	soap_tmp_ns__HBOperate.szXml = szXml;
	soap_begin(soap);
	soap->encodingStyle = "";
	soap_serializeheader(soap);
	soap_serialize_ns__HBOperate(soap, &soap_tmp_ns__HBOperate);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__HBOperate(soap, &soap_tmp_ns__HBOperate, "ns:HBOperate", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__HBOperate(soap, &soap_tmp_ns__HBOperate, "ns:HBOperate", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	szXmlRes = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	if (soap_recv_fault(soap, 1))
		return soap->error;
	soap_tmp_ns__HBOperateResponse = soap_get_ns__HBOperateResponse(soap, NULL, "", NULL);
	if (!soap_tmp_ns__HBOperateResponse || soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	szXmlRes = soap_tmp_ns__HBOperateResponse->szXmlRes;
	return soap_closesock(soap);
}
/* End of client proxy code */
