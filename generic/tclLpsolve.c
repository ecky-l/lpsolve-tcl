/*
 * tclsample.c --
 *
 *	This file implements a Tcl interface to lpsolve
 *
 * See the file "LICENSE" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */

#include <tcl.h>
#include "sample.h"

static int
Lrec_Cmd(
    ClientData clientData,	/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter */
    int objc,			/* Number of arguments */
    Tcl_Obj *const objv[]	/* Argument strings */
    )
{
   return TCL_ERROR;
}

static int
Lpsolve_Cmd(
    ClientData clientData,	/* Not used. */
    Tcl_Interp *interp,		/* Current interpreter */
    int objc,			/* Number of arguments */
    Tcl_Obj *const objv[]	/* Argument strings */
    )
{
   return TCL_ERROR;
}


/*
 *----------------------------------------------------------------------
 *
 * Lpsolve_Init --
 *
 *	Initialize the new package.  The string "Lpsolve" in the
 *	function name must match the PACKAGE declaration at the top of
 *	configure.ac.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The Lpsolve package is created.
 *	One new command "lpsolve" is added to the Tcl interpreter.
 *
 *----------------------------------------------------------------------
 */

int
Lpsolve_Init(Tcl_Interp *interp)
{
#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
        return TCL_ERROR;
    }
#endif //USE_TCL_STUBS

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "lprec", (Tcl_ObjCmdProc *) Lprec_Cmd,
	    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_CreateObjCommand(interp, "lpsolve", (Tcl_ObjCmdProc *) Lpsolve_Cmd,
	    (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);

    return TCL_OK;
}
