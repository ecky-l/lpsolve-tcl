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

static int
Lp_Cmd(
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
static void
LpsolveDeleteNamespace(ClientData data)
{

}

DLLEXPORT int
Lpsolve_Init(Tcl_Interp *interp)
{
    Tcl_Namespace* lpNS = NULL;

#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
        return TCL_ERROR;
    }
#endif //USE_TCL_STUBS

    if ((lpNS = Tcl_CreateNamespace(interp, "lpsolve", (ClientData)NULL, LpsolveDeleteNamespace)) == NULL) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("Cannot create ::lpsolve namespace", -1));
        return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "::lpsolve::lp", (Tcl_ObjCmdProc *) Lp_Cmd,
        (ClientData)NULL, (Tcl_CmdDeleteProc *)NULL);
    Tcl_Export(interp, lpNS, "::lpsolve::lp", 0);

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    return TCL_OK;
}
