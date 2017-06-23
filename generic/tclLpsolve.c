
#include <tcl.h>
#include <lp_lib.h>

typedef struct _tclLPState {
   Tcl_HashTable hash;
   int uid;
} TclLPState;

typedef struct _tclLP {
   const char name[30];
   Tcl_Command commandTkn;
   TclLPState* statePtr;
} TclLP;

static int
LpConstraintCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpObjectiveCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpSetCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpGetCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpSolveCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpSolutionCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpSensitivityCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   return TCL_OK;
}

static int
LpDeleteCmd(TclLP* lp, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   Tcl_DeleteCommandFromToken(interp, lp->commandTkn);
   return TCL_OK;
}


static void
LpsolveCleanup(ClientData data)
{

}

static int
LpLPCmd(ClientData clientData, Tcl_Interp* interp, int objc, Tcl_Obj* const objv[])
{
   int cmdIdx;

   TclLP* lp = (TclLP*) clientData;
   char *subCmds[] = {
      "addvariable", "addconstraint", "configure", "cget", "solve", "solution", "sensitivity", "delete", NULL
   };
   enum BlobIx {
      VariableIx, ConstraintIx, ConfigureIx, CgetIx, SolveIx, SolutionIx, SensitivityIx, DeleteIx
   };

   if (objc == 1 || objc > 4) {
      Tcl_WrongNumArgs(interp, 1, objv, "option ?arg ...?");
      return TCL_ERROR;
   }
   if (Tcl_GetIndexFromObj(interp, objv[1], subCmds, "method", 0, &cmdIdx) != TCL_OK) {
      return TCL_ERROR;
   }

   return TCL_OK;
}

static void
LpDeleteLPCmd(ClientData clientData)
{
   TclLP* lp = (TclLP*)clientData;

   Tcl_HashEntry* entryPtr = Tcl_FindHashEntry(&lp->statePtr->hash, lp->name);
   Tcl_DeleteHashEntry(entryPtr);
   Tcl_Free((char*)lp);
}

static int
LpCreateCmd(ClientData clientData, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
   TclLPState* lpState = (TclLPState*)clientData;
   TclLP* lp;
   Tcl_HashEntry* entryPtr;
   int new;

   lp = (TclLP*)Tcl_Alloc(sizeof(TclLP));
   lp->statePtr = lpState;

   sprintf(lp->name, "::lpsolve::LP%d", lpState->uid);
   lpState->uid++;
   entryPtr = Tcl_CreateHashEntry(&lpState->hash, lp->name, &new);

   lp->commandTkn = Tcl_CreateObjCommand(interp, lp->name, LpLPCmd, lp, LpDeleteLPCmd);
   Tcl_SetHashValue(entryPtr, (ClientData)lp);
   Tcl_SetObjResult(interp, Tcl_NewStringObj(lp->name, -1));

   return TCL_OK;
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
 *	One new command "lpsolve::lp" is added to the Tcl interpreter. It
 *	is used to create, manipulate and solve linear programs
 *
 *----------------------------------------------------------------------
 */
DLLEXPORT int
Lpsolve_Init(Tcl_Interp *interp)
{
    Tcl_Namespace *lpsolveNS = NULL;
    TclLPState* lpState;

#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, "8.1", 0) == NULL) {
        return TCL_ERROR;
    }
#endif //USE_TCL_STUBS

    lpState = (TclLPState*)Tcl_Alloc(sizeof(TclLPState));
    Tcl_InitHashTable(&lpState->hash, TCL_STRING_KEYS);
    lpState->uid = 0;

    if ((lpsolveNS = Tcl_CreateNamespace(interp, "::lpsolve", NULL, NULL)) == NULL) {
        Tcl_SetObjResult(interp, Tcl_NewStringObj("Cannot create ::lpsolve namespace", -1));
        return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "::lpsolve::lp", (Tcl_ObjCmdProc *) LpCreateCmd,
       (ClientData)lpState, (Tcl_CmdDeleteProc *)LpsolveCleanup);
    /*
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_constraint", (Tcl_ObjCmdProc *) LpLookupCmd, "constraint", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_objective", (Tcl_ObjCmdProc *) LpLookupCmd, "objective", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_set", (Tcl_ObjCmdProc *) LpLookupCmd, "set", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_get", (Tcl_ObjCmdProc *) LpLookupCmd, "get", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_solve", (Tcl_ObjCmdProc *) LpLookupCmd, "solve", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_solution", (Tcl_ObjCmdProc *) LpLookupCmd, "solution", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_sensitivity", (Tcl_ObjCmdProc *) LpLookupCmd, "sensitivity", NULL);
    Tcl_CreateObjCommand(interp, "::lpsolve::lp_delete", (Tcl_ObjCmdProc *) LpLookupCmd, "delete", NULL);
    */
    Tcl_Export(interp, lpsolveNS, "lp", 0);

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
        return TCL_ERROR;
    }

    return TCL_OK;
}
