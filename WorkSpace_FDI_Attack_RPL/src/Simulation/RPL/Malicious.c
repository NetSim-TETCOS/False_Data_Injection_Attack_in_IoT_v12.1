#include "main.h"
#include "RPL.h"
#include "RPL_enum.h"
#define MALICIOUS_NODE1 7
#define MALICIOUS_RANK1 3

#define MALICIOUS_NODE2 4
#define MALICIOUS_RANK2 4

/**
Function prototypes
*/
int fn_NetSim_RPL_MaliciousNode(NetSim_EVENTDETAILS*);
void fn_NetSim_RPL_MaliciousRank(NetSim_EVENTDETAILS*);
void rpl_false_data_emulation_injection();
int fn_NetSim_RPL_FreePacket(NetSim_PACKET*);

int fn_NetSim_RPL_MaliciousNode(NetSim_EVENTDETAILS* pstruEventDetails)
{
	if (pstruEventDetails->nDeviceId == MALICIOUS_NODE1)
	{ /*For multiple malicious nodes use if(pstruEventDetails->nDeviceId == MALICIOUS_NODE1 || pstruEventDetails->nDeviceId == MALICIOUS_NODE2)*/
		return 1;
	}
	return 0;
}
void fn_NetSim_RPL_MaliciousRank(NetSim_EVENTDETAILS* pstruEventDetails)
{
	NETSIM_ID receiver = pstruEventDetails->nDeviceId;//receiver id
	PRPL_NODE rpl_r = GET_RPL_NODE(receiver);//receiver node

	switch (pstruEventDetails->pPacket->nControlDataType % 100)
	{
	case DODAG_Information_Object:
		rpl_process_dio_msg();
		if (rpl_r->joined_dodag && pstruEventDetails->nDeviceId == MALICIOUS_NODE1)
		{
			rpl_r->joined_dodag->rank = MALICIOUS_RANK1;
			print_rpl_log("node '%d' MALICIOUS RANK = %d", receiver, rpl_r->joined_dodag->rank);
		}
		else if (rpl_r->joined_dodag && pstruEventDetails->nDeviceId == MALICIOUS_NODE2)
		{
			rpl_r->joined_dodag->rank = MALICIOUS_RANK2;
			print_rpl_log("node '%d' MALICIOUS RANK = %d", receiver, rpl_r->joined_dodag->rank);
		}
		break;
	case Destination_Advertisement_Object:
		rpl_process_dao_msg();
		break;
	case DODAG_Information_Solicitation:
		rpl_process_dis_msg();
		break;
	default:
		fnNetSimError("Unknown rpl ctrl msg %d in %s",pstruEventDetails->pPacket->nControlDataType,__FUNCTION__);
		break;
	}


}

void rpl_false_data_emulation_injection()
{
	if (pstruEventDetails->pPacket->szPayload)
	{
		char a[10] = "100";		//Modify data of the sensor packet
				//char b[100] = "NetSim";	//Add extra event uncomment						if (packet->szPayload->packet[44] == 'i')
		{
			/*Uncomment to add extra Event and che
			for (int i = 75; i <= 80; i++)
			packet->szPayload->packet[i] = b[i - 75];*/
			for (int i = 88; i <= 90; i++)
				pstruEventDetails->pPacket->szPayload->packet[i] = a[i - 88];
		}
	}
	else
	{
		fn_NetSim_RPL_FreePacket(pstruEventDetails->pPacket);
		pstruEventDetails->pPacket = NULL;
	}
}

