/* 
** Copyright (C) 2010 Dirk-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  
**  
*/

#ifndef __MU_RESULT_H__
#define __MU_RESULT_H__

enum _MuResult {
	MU_OK,		/* all went ok */
	MU_IGNORE,	/* ignore this */
	MU_STOP,	/* user wants to stop */
	MU_ERROR	/* some error occured */
};
typedef enum _MuResult MuResult;

#endif /*__MU_RESULT__*/
