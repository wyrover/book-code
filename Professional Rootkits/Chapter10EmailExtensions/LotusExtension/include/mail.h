
#if defined(OS400) && (__OS400_TGTVRM__ >= 510)
#pragma datamodel(P128)
#endif

/*********************************************************************/
/*                                                                   */
/* Licensed Materials - Property of IBM                              */
/*                                                                   */
/* L-GHUS-5VMPGW, L-GHUS-5S3PEE                                      */
/* (C) Copyright IBM Corp. 1989, 2005  All Rights Reserved           */
/*                                                                   */
/* US Government Users Restricted Rights - Use, duplication or       */
/* disclosure restricted by GSA ADP Schedule Contract with           */
/* IBM Corp.                                                         */
/*                                                                   */
/*********************************************************************/


#ifdef __cplusplus
extern "C" {
#endif


/* Mail file definitions */

#ifndef MAIL_DEFS
#define MAIL_DEFS

#ifndef STD_NAME_DEFS
#include "stdnames.h"
#endif

/* Mail note item table indexes (see Message Mailing functions in mailserv.h) */

#define MAIL_SENDTO_ITEM_NUM 0					/* SendTo */
#define	MAIL_COPYTO_ITEM_NUM 1					/* CopyTo */
#define MAIL_FROM_ITEM_NUM 2					/* From */
#define MAIL_SUBJECT_ITEM_NUM 3					/* Subject */
#define MAIL_COMPOSEDDATE_ITEM_NUM 4			/* Composed date */
#define MAIL_POSTEDDATE_ITEM_NUM 5				/* Posted date */
#define MAIL_BODY_ITEM_NUM 6					/* Body */
#define MAIL_INTENDEDRECIPIENT_ITEM_NUM	7		/* Intended recipient */
#define MAIL_FAILUREREASON_ITEM_NUM 8			/* Failure reason */
#define MAIL_RECIPIENTS_ITEM_NUM 9				/* Recipients list */
#define MAIL_ROUTINGSTATE_ITEM_NUM 10			/* Routing state */
#define MAIL_FORM_ITEM_NUM 11					/* Form name */
#define MAIL_SAVED_FORM_ITEM_NUM 12				/* Delivery report saved form name */
#define MAIL_BLINDCOPYTO_ITEM_NUM 13			/* Blind copy to */
#define MAIL_DELIVERYPRIORITY_ITEM_NUM 14		/* Delivery priority */
#define MAIL_DELIVERYREPORT_ITEM_NUM 15			/* Delivery report request  */
#define MAIL_DELIVEREDDATE_ITEM_NUM 16			/* Delivered date */
#define MAIL_DELIVERYDATE_ITEM_NUM 17			/* Delivery date (Confirmation Reports only) */
#define MAIL_CATEGORIES_ITEM_NUM 18				/* Categories */
#define MAIL_FROMDOMAIN_ITEM_NUM 19				/* From domain name */
#define MAIL_SENDTO_LIST_ITEM_NUM 20			/* SendTo TEXT_LIST*/
#define MAIL_RECIPIENTS_LIST_ITEM_NUM 21		/* Recipients TEXT_LIST*/
#define MAIL_RECIP_GROUPS_EXP_ITEM_NUM 22		/* List of recipient group names that have been expanded */
#define MAIL_RETURNRECEIPT_ITEM_NUM 23			/* Return receipt requested? */
#define MAIL_ROUTE_HOPS_ITEM_NUM 24				/* Routing hop count */
#define MAIL_CORRELATION_ITEM_NUM 25			/* Arbitrary delivery report correlation value. */
#define MAIL_ORIGINALPATH_ITEM_NUM 26			/* Original routing path (copy of original message's FromDomain) */
#define MAIL_DELIVER_LOOPS_ITEM_NUM 27			/* Forwarding loop count */
#define MAIL_DEAD_FAILUREREASON_ITEM_NUM 28		
#define MAIL_SMTPORIGINATOR_ITEM_NUM 29
#define MAIL_INETTO_ITEM_NUM 30					/* INetTo */
#define MAIL_INETCC_ITEM_NUM 31					/* INetCc */
#define MAIL_INETBCC_ITEM_NUM 32				/* INetBcc */
#define MAIL_ALTTO_ITEM_NUM 33					/* AltTo */
#define MAIL_ALTCC_ITEM_NUM 34					/* AltCc */
#define MAIL_ALTBCC_ITEM_NUM 35					/* AltBcc */
#define MAIL_DONOTHOLD_ITEM_NUM 36				/* DoNotHold */
#define MAIL_STORAGETO_ITEM_NUM	37				/* Storage type To */
#define MAIL_STORAGECC_ITEM_NUM	38				/* Storage type Cc */
#define MAIL_STORAGEBCC_ITEM_NUM 39				/* Storage type Bcc */
#define MAIL_LANGTO_ITEM_NUM	40				/* Language Tag To */
#define MAIL_LANGCC_ITEM_NUM	41				/* Language Tag Cc */
#define MAIL_LANGBCC_ITEM_NUM 42				/* Language Tag Bcc */
#define MAIL_INETFROM_ITEM_NUM 43
#define MAIL_SMTP_DSN_RCPTS_ITEM_NUM 44
#define MAIL_JOURNAL_FLAG_ITEM_NUM   45			/* $JournalResponsibility flag. */

#define MAIL_ITEMS 46							/* Number of items */

#define MAIL_TO_ITEMS 3							/* Number of To items */
#define MAIL_TO_ITEMS_EXT_ALT	6				/* Extended number of To items (INET and Alt) */
#define MAIL_TO_ITEMS_CACHE_ALT 9				/* Cached number of To items (INET and Alt) */
#define MAIL_TO_ITEMS_EXT 12					/* Extended Number of To items */
#define MAIL_TO_ITEMS_CACHE 15					/* Cached Number of To items */

#define MAIL_EXT_ITEMS_MAX 4					/* Currently INetXX, AltXX, StorageType and Language Tag */
#define MAIL_EXT_INET 0							/* Used for indexing parallel lists - INet */
#define MAIL_EXT_ALT 1							/* Used for indexing parallel lists - Alt */
#define MAIL_EXT_STORAGE 2						/* Used for indexing parallel lists - StorageType */
#define MAIL_EXT_LANG_TAG 3						/* Used for indexing parallel lists - LangTag */

#define MAIL_EXT_ALT_IDX 3						/* Index into extended array for alternate name entries. */
#define MAIL_EXT_STORAGE_IDX  6					/* Index into extended array for storage type */
#define MAIL_EXT_LANG_IDX  9					/* Index into extended array for language tag */

#define MAIL_ADDR_NULL_ENTRY "."				/* Mail address NULL list entry. Make MAIL_ADDR_NULL_ENTRY_CHAR the same! */
#define MAIL_ADDR_NULL_ENTRY_CHAR '.' 			/* Mail address NULL list entry char. */
#define MAIL_ADDR_NULL_ENTRY_SIZE 1				/* Mail address NULL list entry size. */

#define MAIL_LANG_TAG_DEFAULT "en"				/* Default Alternate Name Language Tag string. */
#define MAIL_LANG_TAG_DEFAULT_SIZE 2			/* Default Alternate Name Language Tag string size. */

/* Network domain name parameters */

#define MAIL_DOMAIN_DELIMITER "@"				/* Domain name delimiter string */

#define MAIL_DOMAIN_DELIMITER_NOSPACES 1		/* NOTE: Set this to 1 to revert to padding @'s with spaces */

#if MAIL_DOMAIN_DELIMITER_NOSPACES
#define MAIL_DOMAIN_DELIMITER_SPACES "@"		/* NOTE: Spaces removed in Build 145 because they break SMTP compatibility */
#else
#define MAIL_DOMAIN_DELIMITER_SPACES " @ "		/* Domain name delimiter string */
#endif

#define MAIL_DOMAIN_DELIMITER_CHAR '@'			/* Domain name delimiter character */
#define MAIL_SERVER_DELIMITER_CHAR ':'			/* Server delimiter character within domain specifier (xxx @ LOTUS:CPD-QA_SERVER) */
#define MAXRECIPIENTNAME 256					/* Maximum recipient name length */
#define MAXORIGLEN 12							/* Maximum $Orig logging string length */
#define MAXMESSAGEIDSTRLEN 50					/* Maximum logging message id string length */

/* Internet Address constants */

#define MAIL_822_ADDR_SPEC_BEGIN "<"			/* 822 Internet Address addr-spec begin delimeter string */
#define MAIL_822_ADDR_SPEC_END ">"				/* 822 Internet Address addr-spec end delimeter string */
#define MAIL_822_ADDR_SPEC_BEGIN_CHAR '<'		/* 822 Internet Address addr-spec begin delimeter */
#define MAIL_822_ADDR_SPEC_END_CHAR '>'			/* 822 Internet Address addr-spec end delimeter */
#define MAIL_822_ADDR_COMMENT_BEGIN_CHAR '('	/* 822 Internet Address comment begin delimeter */
#define MAIL_822_ADDR_COMMENT_END_CHAR ')' 		/* 822 Internet Address comment end delimeter */

/* Mail router mailbox file */

#define MAILBOX_NAME "mail.box"
#define MAILBOX_ROOT_NAME "mail"
#define MAILBOX_TEMPLATE_NAME "mailbox.ntf"
/* If you change the value of MAIL_TEMPLATE_NAME, you must update the value of the entry 'mail_template' in the following two files:
 * java\lotus\notes\addins\ispy\ISpyResources.properties
 * java\lotus\notes\addins\ispy\ISpyResources_en_US.properties
 */
#define MAIL_TEMPLATE_NAME "mail7.ntf"
#define MAILEX_TEMPLATE_NAME "mail7ex.ntf"
#define MAIL_INOTES_TEMPLATE_NAME "dwa7.ntf"
#define ADDRESSBOOK_NAME "names.nsf"
#define PUBLIC_ADDRESSBOOK_TEMPLATE_NAME "pubnames.ntf"
#define PERSONAL_ADDRESSBOOK_TEMPLATE_NAME "pernames.ntf"

/* SMTP MTA mailbox file */
#define SMTPMAILBOX_NAME "smtp.box"

/* X.400 MTA mailbox file */
#define X400MAILBOX_NAME "x400.box"
#endif

#ifdef __cplusplus
}
#endif


#if defined(OS400) && (__OS400_TGTVRM__ >= 510)
#pragma datamodel(pop)
#endif

