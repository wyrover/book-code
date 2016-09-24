#include <windows.h>
#include "mmCom.h"
#include "../../../../3rdparty/tinyxml/tinyxml.h"


int loadmmconfig(s_mm_config* mmcfg,void* pbuf)
{
    if (!mmcfg || !pbuf)
		return -1;
    for (int i=0;i<sizeof(mmcfg->m_scpenv)/sizeof(char*);i++)
	{
        if (mmcfg->m_scpenv.m_envs[i])
		{
            free(mmcfg->m_scpenv.m_envs[i]);
            mmcfg->m_scpenv.m_envs[i] = 0;
		}
	}
	s_mm_config* pmmcfg2 = (s_mm_config*)pbuf;
    *mmcfg = *pmmcfg2;
    mmcfg->m_szsection = 0;
	int pipos = 0;
	for (int i=0;i<sizeof(pmmcfg2->m_scpenv.m_envs)/sizeof(char*);i++)
	{
		pipos = (int)pmmcfg2->m_scpenv.m_envs[i];
		if (pipos > 0)
		{
			char* ppstr = (char*)pbuf+pipos;
            mmcfg->m_scpenv.m_envs[i] = (char*)malloc(strlen(ppstr)+1);
            if (mmcfg->m_scpenv.m_envs[i])
                strcpy(mmcfg->m_scpenv.m_envs[i],ppstr);
		}
	}
	pipos = (int)pmmcfg2->m_ptselfcp;
	if (pipos > 0 && pmmcfg2->m_szselfcp > 0)
	{
        mmcfg->m_ptselfcp = (char*)malloc(pmmcfg2->m_szselfcp);
        if (mmcfg->m_ptselfcp)
		{
            mmcfg->m_szselfcp = pmmcfg2->m_szselfcp;
            memcpy(mmcfg->m_ptselfcp,(char*)pmmcfg2+pipos,mmcfg->m_szselfcp);
		}else
            mmcfg->m_szselfcp = 0;
	}
	return 0;
}


char* bufmmconfig(s_mm_config* mmcfg,int& pszbuf,const char* pcurdir)
{
	pszbuf = 0;
    if (!mmcfg)
		return 0;
	pszbuf = sizeof(s_mm_config);
    if (mmcfg->m_ptselfcp && mmcfg->m_szselfcp > 0)
        pszbuf += mmcfg->m_szselfcp;
    for (int i=0;i<sizeof(mmcfg->m_scpenv.m_envs)/sizeof(char*);i++)
	{
        const char* ppstr = mmcfg->m_scpenv.m_envs[i];
		if (ppstr)
		{
			int pslen = strlen(ppstr);
			if (i!=5 && pcurdir)
			{
				int pscur = strlen(pcurdir);
				for (int x=0,j=0;j<=pslen;j++)
				{
					if (ppstr[j] == ';' || ppstr[j] == 0)
					{
						const char* psubstr = ppstr+x;
						int	  psublen = j-x;
						if (psublen > 0 || psublen < 2 || psubstr[1] != ':')
							pszbuf += pscur;
						x=j+1;
					}
				}
			}
			pszbuf += pslen+1;
		}
	}
	char* pbuf = (char*)malloc(pszbuf);
	if (!pbuf)
		return 0;
	s_mm_config* pmmcfg2 = (s_mm_config*)pbuf;
    *pmmcfg2 = *mmcfg;
	int pipos = sizeof(s_mm_config);
    for (int i=0;i<sizeof(mmcfg->m_scpenv.m_envs)/sizeof(char*);i++)
	{
        char* ptstr = mmcfg->m_scpenv.m_envs[i];
		if (ptstr)
		{
			pmmcfg2->m_scpenv.m_envs[i] = (char*)pipos;

			int pslen = strlen(ptstr);
			if (i!=5 && pcurdir)
			{
				int pscur = strlen(pcurdir);
				for (int x=0,j=0;j<=pslen;j++)
				{
					if (ptstr[j] == ';' || ptstr[j] == 0)
					{
						const char* psubstr = ptstr+x;
						int	  psublen = j-x;
						if (psublen > 0 || psublen < 2 || psubstr[1] != ':')
						{
							memcpy(pbuf+pipos,pcurdir,pscur);
							pipos += pscur;
						}
						memcpy(pbuf+pipos,psubstr,psublen+1);
						pipos+=psublen+1;
						x=j+1;
					}
				}
			}else
			{
				memcpy(pbuf+pipos,ptstr,pslen+1);
				pipos+=pslen+1;
			}
		}
	}
    if (mmcfg->m_ptselfcp && mmcfg->m_szselfcp > 0)
	{
		pmmcfg2->m_ptselfcp = (char*)pipos;
        pmmcfg2->m_szselfcp = mmcfg->m_szselfcp;
        memcpy(pbuf+pipos,mmcfg->m_ptselfcp,mmcfg->m_szselfcp);
        pipos+=mmcfg->m_szselfcp;
	}
	pmmcfg2->m_szsection = pipos;
	return pbuf;
}

int savemmconfigxml(s_mm_config* pmmconfig,const char* pxmlfile)
{
	if (!pmmconfig)
		return -1;
	char pstmp[16];
	TiXmlElement pXmlEnt("mmGhost");
	pXmlEnt.SetAttribute("virtualdll",pmmconfig->m_virtualdll);
	pXmlEnt.SetAttribute("int3cmd",pmmconfig->m_int3cmd);
	sprintf(pstmp,"%08X",pmmconfig->m_int3ecode);
	pXmlEnt.SetAttribute("int3ecode",pstmp);
	pXmlEnt.SetAttribute("stepcmd",pmmconfig->m_stepcmd);
	sprintf(pstmp,"%08X",pmmconfig->m_stepecode);
	pXmlEnt.SetAttribute("stepecode",pstmp);
	pXmlEnt.SetAttribute("singletd",pmmconfig->m_singletd);
	if (pmmconfig->m_selfdll)
	{
		TiXmlElement	pselfdll("selfdll");
		pselfdll.InsertEndChild(TiXmlText(pmmconfig->m_selfdll));
		pXmlEnt.InsertEndChild(pselfdll);
	}
	TiXmlElement pXmlScp("script");
	for (int i=0;i<sizeof(pmmconfig->m_scpenv.m_envs)/sizeof(char*);i++)
	{
		char* ptstr = pmmconfig->m_scpenv.m_envs[i];
		if (ptstr)
		{
			TiXmlElement* pXmlSubEnt = 0;
			switch (i)
			{
			case 0:pXmlSubEnt = new TiXmlElement("logfile");break;
			case 1:pXmlSubEnt = new TiXmlElement("crtpath");break;
			case 2:pXmlSubEnt = new TiXmlElement("syspath");break;
			case 3:pXmlSubEnt = new TiXmlElement("incpath");break;
			case 4:pXmlSubEnt = new TiXmlElement("libpath");break;
			case 5:pXmlSubEnt = new TiXmlElement("libfiles");break;
			case 6:pXmlSubEnt = new TiXmlElement("scpfiles");break;
			}
			if (pXmlSubEnt)
			{
				pXmlSubEnt->InsertEndChild(TiXmlText(ptstr));
				pXmlScp.InsertEndChild(*pXmlSubEnt);
				delete pXmlSubEnt;
			}
		}
	}

	pXmlEnt.InsertEndChild(pXmlScp);
	TiXmlDocument pXmlSave;
	pXmlSave.Parse("<?xml version=\"1.0\" encoding=\"GB2312\"?>");
	pXmlSave.InsertEndChild(pXmlEnt);
	pXmlSave.SaveFile(pxmlfile);
	return 0;
}
int loadmmconfigxml(s_mm_config* pmmconfig,const char* pxmlfile)
{
	if (!pmmconfig)
		return -1;
	TiXmlDocument pXmlDoc;
	if (!pXmlDoc.LoadFile(pxmlfile))
		return -1;
	TiXmlElement* pXmlEnt = pXmlDoc.FirstChildElement("mmGhost");
	if (pXmlEnt)
	{
		const char* ptmpstr;
		ptmpstr = pXmlEnt->Attribute("virtualdll");
		pmmconfig->m_virtualdll = ptmpstr?atoi(ptmpstr):0;
		ptmpstr = pXmlEnt->Attribute("int3cmd");
		pmmconfig->m_int3cmd	= ptmpstr?atoi(ptmpstr):0;
		ptmpstr = pXmlEnt->Attribute("int3ecode");
		ptmpstr?sscanf(ptmpstr,"%X",&pmmconfig->m_int3ecode):0;
		ptmpstr = pXmlEnt->Attribute("stepcmd");
		pmmconfig->m_stepcmd	= ptmpstr?atoi(ptmpstr):0;
		ptmpstr = pXmlEnt->Attribute("stepecode");
		ptmpstr?sscanf(ptmpstr,"%X",&pmmconfig->m_stepecode):0;
		ptmpstr = pXmlEnt->Attribute("singletd");
		pmmconfig->m_singletd = ptmpstr?atoi(ptmpstr):0;
		TiXmlElement* pSubEnt = pXmlEnt->FirstChildElement("selfdll");
		if (pSubEnt)
		{
			const char* ptstr = pSubEnt->GetText();
			if (ptstr)
			{
				pmmconfig->m_selfdll = (char*)malloc(strlen(ptstr)+1);
				if (pmmconfig->m_selfdll)
				{
					strcpy(pmmconfig->m_selfdll,ptstr);
					FILE* pf = fopen(pmmconfig->m_selfdll,"rb");
					if (pf)
					{
						fseek(pf,0,FILE_END);
						long pszfile = ftell(pf);
						if (pszfile > 0)
						{
							pmmconfig->m_ptselfcp = (char*)malloc(pszfile);
							if (pmmconfig->m_ptselfcp)
							{
								fseek(pf,0,FILE_BEGIN);
								fread(pmmconfig->m_ptselfcp,pszfile,1,pf);
								pmmconfig->m_szselfcp = pszfile;
							}
						}
						fclose(pf);
					}
				}
			}
		}
	}
	TiXmlElement* pXmlEntScp = pXmlEnt->FirstChildElement("script");
	if (pXmlEntScp)
	{
		for (int i=0;i<sizeof(pmmconfig->m_scpenv.m_envs)/sizeof(char*);i++)
		{
			TiXmlElement* pSubEnt = 0;
			switch (i)
			{
			case 0:pSubEnt = pXmlEntScp->FirstChildElement("logfile");break;
			case 1:pSubEnt = pXmlEntScp->FirstChildElement("crtpath");break;
			case 2:pSubEnt = pXmlEntScp->FirstChildElement("syspath");break;
			case 3:pSubEnt = pXmlEntScp->FirstChildElement("incpath");break;
			case 4:pSubEnt = pXmlEntScp->FirstChildElement("libpath");break;
			case 5:pSubEnt = pXmlEntScp->FirstChildElement("libfiles");break;
			case 6:pSubEnt = pXmlEntScp->FirstChildElement("scpfiles");break;
			}
			if (pSubEnt)
			{
				const char* ptstr = pSubEnt->GetText();
				if (ptstr)
				{
					char* pstrcp = (char*)malloc(strlen(ptstr)+1);
					if (pstrcp)
					{
						strcpy(pstrcp,ptstr);
						pmmconfig->m_scpenv.m_envs[i] = pstrcp;
					}
				}
			}
		}
	}
	return 0;
}
