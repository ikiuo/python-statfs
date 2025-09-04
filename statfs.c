#ifndef PY_SSIZE_T_CLEAN
#define PY_SSIZE_T_CLEAN
#endif /* PY_SSIZE_T_CLEAN */

#include <Python.h>

#define PyVER_VALUE(major, minor)  (((major) << 24) | ((minor) << 16))
#define PyVER_OLDER(major, minor)  (PY_VERSION_HEX <  PyVER_VALUE((major), (minor)))
#define PyVER_EQUAL(major, minor)  (PY_VERSION_HEX == PyVER_VALUE((major), (minor)))
#define PyVER_NEWER(major, minor)  (PY_VERSION_HEX >= PyVER_VALUE((major), (minor)))

/*
 *
 */

#include "config.h"

#ifdef COMPILE_DARWIN
#  ifdef _DARWIN_FEATURE_64_BIT_INODE
#    define USE_STATFS_DF64 1
#  else
#    define USE_STATFS_DF32 1
#  endif
#endif /* DARWIN */

/*
 *
 */

#include <sys/param.h>
#include <sys/mount.h>

#ifndef FALSE
#define FALSE (!!0)
#endif
#ifndef TRUE
#define TRUE (!0)
#endif

typedef struct statfs statfs_t;

/*
 *
 */

inline static PyObject *
NewNone()
{
    Py_RETURN_NONE;
}

inline static void
DecRelease(PyObject **pobj)
{
    Py_DecRef(*pobj);
    *pobj = NULL;
}

inline static void
TupleMoveItem(PyObject *tuple, Py_ssize_t index, PyObject **pobj)
{
    PyTuple_SET_ITEM(tuple, index, *pobj);
    *pobj = NULL;
}

inline static void
ListMoveItem(PyObject *tuple, Py_ssize_t index, PyObject **pobj)
{
    PyList_SET_ITEM(tuple, index, *pobj);
    *pobj = NULL;
}

inline static int
ModuleAddRelease(PyObject *module, const char *name, PyObject **value)
{
    int res;

#if PyVER_OLDER(3, 13)
    res = PyModule_AddObject(module, name, *value);
#else  /* >= 3.13 */
    res = PyModule_Add(module, name, *value);
#endif
    if (res >= 0)
        *value = NULL;
    return res;
}

/*
 *
 */

#define STATFS_MEMBERS 31

static const char *statfs_member[STATFS_MEMBERS][2] = {
    { "f_version",     "F" }, /* FBSD */
    { "f_otype",       "d" }, /* DF32 */
    { "f_oflags",      "d" }, /* DF32 */
    { "f_flags",       "-" },
    { "f_flags_ext",   "D" }, /* DF64 */
    { "f_owner",       "-" },

    { "f_fsid",        "-" },
    { "f_type",        "-" },
    { "f_fssubtype",   "D" }, /* DF64 */
    { "f_namemax",     "F" }, /* FBSD */

    { "f_fstypename",  "-" },
    { "f_mntfromname", "-" },
    { "f_mntonname",   "-" },
    { "f_spare",       "F" }, /* FBSD */
    { "f_charspare",   "F" }, /* FBSD */

    { "f_iosize",      "-" },
    { "f_bsize",       "-" },
    { "f_blocks",      "-" },
    { "f_bavail",      "-" },
    { "f_bfree",       "-" },

    { "f_ffree",       "-" },
    { "f_files",       "-" },

    { "f_syncwrites",  "F" }, /* FBSD */
    { "f_asyncwrites", "F" }, /* FBSD */
    { "f_syncreads",   "F" }, /* FBSD */
    { "f_asyncreads",  "F" }, /* FBSD */

    { "f_reserved",    "D" }, /* DF64 */
    { "f_reserved1",   "d" }, /* DF32 */
    { "f_reserved2",   "d" }, /* DF32 */
    { "f_reserved3",   "d" }, /* DF32 */
    { "f_reserved4",   "d" }, /* DF32 */
};

typedef struct statfs_args {
    PyObject *version;     /* FBSD */
    PyObject *otype;       /* DF32 */
    PyObject *oflags;      /* DF32 */
    PyObject *flags;
    PyObject *flags_ext;   /* DF64 */
    PyObject *owner;

    PyObject *fsid;
    PyObject *type;
    PyObject *fssubtype;   /* DF64 */
    PyObject *namemax;     /* FBSD */

    PyObject *fstypename;
    PyObject *mntfromname;
    PyObject *mntonname;
    PyObject *spare;       /* FBSD */
    PyObject *charspare;   /* FBSD */

    PyObject *iosize;
    PyObject *bsize;
    PyObject *blocks;
    PyObject *bavail;
    PyObject *bfree;

    PyObject *ffree;
    PyObject *files;

    PyObject *syncwrites;  /* FBSD */
    PyObject *asyncwrites; /* FBSD */
    PyObject *syncreads;   /* FBSD */
    PyObject *asyncreads;  /* FBSD */

    PyObject *reserved;    /* DF64 */
    PyObject *reserved1;   /* DF32 */
    PyObject *reserved2;   /* DF32 */
    PyObject *reserved3;   /* DF32 */
    PyObject *reserved4;   /* DF32 */

    PyObject *fsidv[2];
    PyObject *reservedv[8];
    PyObject *reserved2v[2];
    PyObject *reserved4v[4];

} statfs_args;

static PyObject *new_statfs_func = NULL;

static void
statfs_init(statfs_args *o)
{
    int i;

    o->version = NULL;
    o->otype = NULL;
    o->oflags = NULL;
    o->flags = NULL;
    o->flags_ext = NULL;
    o->owner = NULL;

    o->fsid = NULL;
    o->type = NULL;
    o->fssubtype = NULL;
    o->namemax = NULL;

    o->fstypename = NULL;
    o->mntfromname = NULL;
    o->mntonname = NULL;
    o->spare = NULL;
    o->charspare = NULL;

    o->iosize = NULL;
    o->bsize = NULL;
    o->blocks = NULL;
    o->bavail = NULL;
    o->bfree = NULL;

    o->ffree = NULL;
    o->files = NULL;

    o->syncwrites = NULL;
    o->asyncwrites = NULL;
    o->syncreads = NULL;
    o->asyncreads = NULL;

    o->reserved = NULL;
    o->reserved1 = NULL;
    o->reserved2 = NULL;
    o->reserved3 = NULL;
    o->reserved4 = NULL;

    for (i = 0; i < 2; ++i)
        o->fsidv[i] = NULL;
    for (i = 0; i < 8; ++i)
        o->reservedv[i] = NULL;
    for (i = 0; i < 2; ++i)
        o->reserved2v[i] = NULL;
    for (i = 0; i < 4; ++i)
        o->reserved4v[i] = NULL;
}

static void
statfs_exit(statfs_args *o)
{
    int i;

    Py_XDECREF(o->version);
    Py_XDECREF(o->otype);
    Py_XDECREF(o->oflags);
    Py_XDECREF(o->flags);
    Py_XDECREF(o->flags_ext);
    Py_XDECREF(o->owner);

    Py_XDECREF(o->fsid);
    Py_XDECREF(o->type);
    Py_XDECREF(o->fssubtype);
    Py_XDECREF(o->namemax);

    Py_XDECREF(o->fstypename);
    Py_XDECREF(o->mntfromname);
    Py_XDECREF(o->mntonname);
    Py_XDECREF(o->spare);
    Py_XDECREF(o->charspare);

    Py_XDECREF(o->iosize);
    Py_XDECREF(o->bsize);
    Py_XDECREF(o->blocks);
    Py_XDECREF(o->bavail);
    Py_XDECREF(o->bfree);

    Py_XDECREF(o->ffree);
    Py_XDECREF(o->files);

    Py_XDECREF(o->syncwrites);
    Py_XDECREF(o->asyncwrites);
    Py_XDECREF(o->syncreads);
    Py_XDECREF(o->asyncreads);

    Py_XDECREF(o->reserved);
    Py_XDECREF(o->reserved1);
    Py_XDECREF(o->reserved2);
    Py_XDECREF(o->reserved3);
    Py_XDECREF(o->reserved4);

    for (i = 0; i < 2; ++i)
        Py_XDECREF(o->fsidv[i]);
    for (i = 0; i < 8; ++i)
        Py_XDECREF(o->reservedv[i]);
    for (i = 0; i < 2; ++i)
        Py_XDECREF(o->reserved2v[i]);
    for (i = 0; i < 4; ++i)
        Py_XDECREF(o->reserved4v[i]);
}

static void
statfs_fix(statfs_args *o)
{
#define statfs_fix_none(n) if (!o->n) o->n = NewNone()

    /* FBSD */
    statfs_fix_none(version);
    statfs_fix_none(namemax);
    statfs_fix_none(spare);
    statfs_fix_none(charspare);
    statfs_fix_none(syncwrites);
    statfs_fix_none(asyncwrites);
    statfs_fix_none(syncreads);
    statfs_fix_none(asyncreads);

    /* DF32 */
    statfs_fix_none(otype);
    statfs_fix_none(oflags);
    statfs_fix_none(reserved1);
    statfs_fix_none(reserved2);
    statfs_fix_none(reserved3);
    statfs_fix_none(reserved4);

    /* DF64 */
    statfs_fix_none(flags_ext);
    statfs_fix_none(fssubtype);
    statfs_fix_none(reserved);

#undef statfs_fix_none
}

static PyObject *
build_statfs(statfs_t *pmnt)
{
    statfs_args sa;
    statfs_args *o = &sa;
    PyObject *pinfo = NULL;
    PyObject *args = NULL;
    int i = 0;

#ifdef USE_STATFS_DF64
    int rcnt = 0;
#endif /* USE_STATFS_DF64 */

    statfs_init(o);

#define build_statfs_gen_str(n) if (!(o->n = PyUnicode_FromString((const char *)pmnt->f_##n))) goto exit
#define build_statfs_gen_l(n)   if (!(o->n = PyLong_FromLong((long) pmnt->f_##n))) goto exit
#define build_statfs_gen_ul(n)  if (!(o->n = PyLong_FromUnsignedLong((unsigned long) pmnt->f_##n))) goto exit
#define build_statfs_gen_ull(n) if (!(o->n = PyLong_FromUnsignedLongLong((unsigned long long) pmnt->f_##n))) goto exit

    build_statfs_gen_ull(flags);
    build_statfs_gen_l(owner);

    {/* f_fsid */
        if (!(o->fsidv[0] = PyLong_FromLong(pmnt->f_fsid.val[0]))) goto exit;
        if (!(o->fsidv[1] = PyLong_FromLong(pmnt->f_fsid.val[1]))) goto exit;
        if (!(o->fsid = PyTuple_New(2))) goto exit;
        TupleMoveItem(o->fsid, 0, &o->fsidv[0]);
        TupleMoveItem(o->fsid, 1, &o->fsidv[1]);
    }
    build_statfs_gen_ull(type);

    build_statfs_gen_str(fstypename);
    build_statfs_gen_str(mntfromname);
    build_statfs_gen_str(mntonname);

    build_statfs_gen_ull(iosize);
    build_statfs_gen_ull(bsize);
    build_statfs_gen_ull(blocks);
    build_statfs_gen_ull(bavail);
    build_statfs_gen_ull(bfree);

    build_statfs_gen_ull(ffree);
    build_statfs_gen_ull(files);

#ifdef COMPILE_FREEBSD
    build_statfs_gen_ul(version);
    build_statfs_gen_ul(namemax);
    build_statfs_gen_str(spare);
    build_statfs_gen_str(charspare);
    build_statfs_gen_ull(syncwrites);
    build_statfs_gen_ull(asyncwrites);
    build_statfs_gen_ull(syncreads);
    build_statfs_gen_ull(asyncreads);
#endif /* COMPILE_FREEBSD */
#ifdef USE_STATFS_DF32
    build_statfs_gen_l(otype);
    build_statfs_gen_l(oflags);
    build_statfs_gen_l(reserved1);
    {/* f_reserved2 */
        if (!(o->reserved2v[0] = PyLong_FromLong(pmnt->f_reserved2[0]))) goto exit;
        if (!(o->reserved2v[1] = PyLong_FromLong(pmnt->f_reserved2[1]))) goto exit;
        if (!(o->reserved2 = PyTuple_New(2))) goto exit;
        TupleMoveItem(o->reserved2, 0, &o->reserved2v[0]);
        TupleMoveItem(o->reserved2, 1, &o->reserved2v[1]);
    }
    build_statfs_gen_l(reserved3);
    {/* f_reserved4 */
        for (i = 0; i < 4; ++i)
            if (!(o->reserved4v[i] = PyLong_FromLong(pmnt->f_reserved4[i])))
                goto exit;
        if (!(o->reserved4 = PyTuple_New(4)))
            goto exit;
        for (i = 0; i < 4; ++i)
            TupleMoveItem(o->reserved4, i, &o->reserved4v[i]);
    }
#endif /* USE_STATFS_DF32 */
#ifdef USE_STATFS_DF64
    build_statfs_gen_ul(flags_ext);
    build_statfs_gen_ul(fssubtype);
    {
        rcnt = (int)(sizeof(pmnt->f_reserved) / sizeof(pmnt->f_reserved[0]));

        for (i = 0; i < rcnt; ++i)
            if (!(o->reservedv[i] = PyLong_FromUnsignedLong(pmnt->f_reserved[i])))
                goto exit;
        if (!(o->reserved = PyTuple_New(rcnt)))
            goto exit;
        for (i = 0; i < rcnt; ++i)
            TupleMoveItem(o->reserved, i, &o->reservedv[i]);
    }
#endif /* USE_STATFS_DF64 */

#undef build_statfs_gen_str
#undef build_statfs_gen_l
#undef build_statfs_gen_ul
#undef build_statfs_gen_ull

    statfs_fix(o);
    if (!(args = PyTuple_New(STATFS_MEMBERS)))
        goto exit;

#define build_statfs_arg(n) TupleMoveItem(args, (Py_ssize_t) i, &o->n); ++i

    i = 0;
    build_statfs_arg(version);
    build_statfs_arg(otype);
    build_statfs_arg(oflags);
    build_statfs_arg(flags);
    build_statfs_arg(flags_ext);
    build_statfs_arg(owner);

    build_statfs_arg(fsid);
    build_statfs_arg(type);
    build_statfs_arg(fssubtype);
    build_statfs_arg(namemax);

    build_statfs_arg(fstypename);
    build_statfs_arg(mntfromname);
    build_statfs_arg(mntonname);
    build_statfs_arg(spare);
    build_statfs_arg(charspare);

    build_statfs_arg(iosize);
    build_statfs_arg(bsize);
    build_statfs_arg(blocks);
    build_statfs_arg(bavail);
    build_statfs_arg(bfree);

    build_statfs_arg(ffree);
    build_statfs_arg(files);

    build_statfs_arg(syncwrites);
    build_statfs_arg(asyncwrites);
    build_statfs_arg(syncreads);
    build_statfs_arg(asyncreads);

    build_statfs_arg(reserved);
    build_statfs_arg(reserved1);
    build_statfs_arg(reserved2);
    build_statfs_arg(reserved3);
    build_statfs_arg(reserved4);

    if (i != STATFS_MEMBERS)
        abort(); /* Bug!! */

#undef build_statfs_arg

    pinfo = PyObject_Call(new_statfs_func, args, NULL);
exit:
    statfs_exit(o);
    return pinfo;
}

static PyObject *
method_statfs(PyObject *module, PyObject *args, PyObject *kwargs)
{
#if HAVE_STATFS

    static char *keywords[] = { "path", NULL };

    PyObject *name = NULL;
    statfs_t buf;

    (void) module;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O", keywords, &name))
        return NULL;
    if (!name || !PyUnicode_Check(name))
    {
        PyErr_BadArgument();
        return NULL;
    }
    if (statfs(PyUnicode_AsUTF8AndSize(name, NULL), &buf) < 0)
        return PyErr_SetFromErrno(PyExc_OSError);
    return build_statfs(&buf);

#else  /* !HAVE_STATFS */

    (void) module;
    (void) args;
    (void) kwargs;

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;

#endif /* !HAVE_STATFS */
}

static PyObject *
method_fstatfs(PyObject *module, PyObject *args, PyObject *kwargs)
{
#if HAVE_STATFS

    static char *keywords[] = { "fd", NULL };

    int fd = -1;
    statfs_t buf;

    (void) module;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "i", keywords, &fd))
        return NULL;
    if (fstatfs(fd, &buf) < 0)
        return PyErr_SetFromErrno(PyExc_OSError);
    return build_statfs(&buf);

#else  /* !HAVE_FSTATFS */

    (void) module;
    (void) args;
    (void) kwargs;

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;

#endif /* !HAVE_FSTATFS */
}

static PyObject *
method_getfsstat(PyObject *module, PyObject *args, PyObject *kwargs)
{
#if HAVE_GETFSSTAT
    static char *keywords[] = { "flags", NULL };

    statfs_t *pbuf = NULL;
    PyObject *plist = NULL;
    PyObject *pinfo = NULL;
    int success = FALSE;
    int flags = MNT_NOWAIT;
    int bufsize = 0;
    int mcnt = 0;
    int i = 0;

    (void) module;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i", keywords, &flags))
        return NULL;

    mcnt = getfsstat(NULL, 0, flags);
    if (mcnt < 0)
        return PyErr_SetFromErrno(PyExc_OSError);
    bufsize = (int) (sizeof(statfs_t) * mcnt);
    pbuf = (statfs_t *) malloc(bufsize);
    if (!pbuf)
        return PyErr_NoMemory();
    mcnt = getfsstat(pbuf, bufsize, flags);
    if (mcnt < 0)
    {
        PyErr_SetFromErrno(PyExc_OSError);
        free(pbuf);
        /* pbuf = NULL; */
        return NULL;
    }

    if ((plist = PyList_New(mcnt)))
    {
        for (i = 0; i < mcnt; ++i)
        {
            if (!(pinfo = build_statfs(pbuf + i)))
                break;
            ListMoveItem(plist, (Py_ssize_t)i, &pinfo);
        }
    }
    success = (i == mcnt);

    if (!success)
    {
        Py_XDECREF(plist);
        plist = NULL;
    }
    free(pbuf);
    /* pbuf = NULL; */
    return plist;

#else  /* !HAVE_GETFSSTAT */

    (void) module;
    (void) args;
    (void) kwargs;

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;

#endif /* !HAVE_GETFSSTAT */
}

static PyObject *
method_getmntinfo(PyObject *module, PyObject *args, PyObject *kwargs)
{
#if HAVE_GETMNTINFO

    static char *keywords[] = { "flags", NULL };

    statfs_t *pbuf = NULL;
    PyObject *plist = NULL;
    PyObject *pinfo = NULL;
    int success = FALSE;
    int flags = MNT_NOWAIT;
    int mcnt = 0;
    int i = 0;

    (void) module;

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "|i", keywords, &flags))
        return NULL;

#ifdef HAVE_GETMNTINFO_R_NP
    mcnt = getmntinfo_r_np(&pbuf, flags);
#else  /* !HAVE_GETMNTINFO_R_NP */
    mcnt = getmntinfo(&pbuf, flags);
#endif /* !HAVE_GETMNTINFO_R_NP */
    if (mcnt < 0)
        return PyErr_SetFromErrno(PyExc_OSError);
    if ((plist = PyList_New(mcnt)))
    {
        for (i = 0; i < mcnt; ++i)
        {
            if (!(pinfo = build_statfs(pbuf + i)))
                break;
            ListMoveItem(plist, (Py_ssize_t)i, &pinfo);
        }
    }
    success = (i == mcnt);
#ifdef HAVE_GETMNTINFO_R_NP
    if (pbuf)
    {
        free(pbuf);
        pbuf = NULL;
    }
#endif /* HAVE_GETMNTINFO_R_NP */

    if (!success)
    {
        Py_XDECREF(plist);
        plist = NULL;
    }
    return plist;

#else /* !HAVE_GETMNTINFO */

    (void) module;
    (void) args;
    (void) kwargs;

    PyErr_SetNone(PyExc_NotImplementedError);
    return NULL;

#endif /* !HAVE_GETMNTINFO */
}

/*
 *
 */

static int
prepare_statfs(PyObject *module, PyObject *namedtuple)
{
    PyObject *name = NULL;
    PyObject *item = NULL;
    PyObject *indices = NULL;
    PyObject *members = NULL;
    const char *mflag = NULL;
    const char *valid[STATFS_MEMBERS];
    int index[STATFS_MEMBERS];
    const char *model;
    int cnt, c;
    int i;

    /* model */

#if defined(COMPILE_FREEBSD)
    model = "FreeBSD";
#elif defined(USE_STATFS_DF64)
    model = "Darwin64";
#elif defined(USE_STATFS_DF32)
    model = "Darwin32";
#else  /* unknown */
    model = "unknown";
#endif
    if (!(name = PyUnicode_FromString(model)))
        goto error;
    if (ModuleAddRelease(module, "model", &name) < 0)
        goto error;

    /* members, statfs() */

    if (!(members = PyTuple_New(STATFS_MEMBERS)))
        goto error;
    for (i = 0; i < STATFS_MEMBERS; ++i)
    {
        if (!(item = PyUnicode_FromString(statfs_member[i][0])))
            goto error;
        TupleMoveItem(members, (Py_ssize_t) i, &item);
    }
    if (!(name = PyUnicode_FromString("statfs")))
        goto error;
    new_statfs_func = PyObject_CallFunctionObjArgs(namedtuple, name, members, NULL);
    DecRelease(&name);
    if (!new_statfs_func)
        goto error;
    if (ModuleAddRelease(module, "members", &members) < 0)
        goto error;

    /* valid */

    cnt = 0;
    for (i = 0; i < STATFS_MEMBERS; ++i)
    {
        mflag = statfs_member[i][1];
        while ((c = *mflag++))
        {
            if (c == '-')
                break;
#ifdef COMPILE_FREEBSD
            else if (c == 'F')
                break;
#endif /* FBSD */
#ifdef USE_STATFS_DF64
            else if (c == 'D')
                break;
#endif /* DF64 */
#ifdef USE_STATFS_DF32
            else if (c == 'd')
                break;
#endif /* DF32 */
        }
        if (c)
        {
            valid[cnt] = statfs_member[i][0];
            index[cnt] = i;
            ++cnt;
        }
    }

    if (!(members = PyTuple_New(cnt))) goto error;
    if (!(indices = PyTuple_New(cnt))) goto error;
    for (i = 0; i < cnt; ++i)
    {
        if (!(name = PyUnicode_FromString(valid[i]))) goto error;
        if (!(item = PyLong_FromLong(index[i]))) goto error;
        TupleMoveItem(members, (Py_ssize_t) i, &name);
        TupleMoveItem(indices, (Py_ssize_t) i, &item);
    }
    if (ModuleAddRelease(module, "valid_members", &members) < 0) goto error;
    if (ModuleAddRelease(module, "valid_indices", &indices) < 0) goto error;

    return TRUE;

error:
    Py_XDECREF(name);
    Py_XDECREF(item);
    Py_XDECREF(indices);
    Py_XDECREF(members);
    return FALSE;
}

static int
prepare_namedtuple(PyObject *module)
{
    PyObject *collections = NULL;
    PyObject *namedtuple = NULL;
    int res = 0;

    collections = PyImport_ImportModule("collections");
    if (!collections)
        return FALSE;
    namedtuple = PyObject_GetAttrString(collections, "namedtuple");
    Py_DecRef(collections);
    if (!namedtuple)
        return FALSE;
    res = prepare_statfs(module, namedtuple);
    Py_DecRef(namedtuple);
    return res;
}

static int
prepare_module(PyObject *module)
{
    if (!prepare_namedtuple(module)) return FALSE;

    /**/

    if (PyModule_AddIntMacro(module, MNT_WAIT) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_DWAIT) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_NOWAIT) < 0) return FALSE;

    /**/

    if (PyModule_AddIntMacro(module, MNT_RDONLY) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_SYNCHRONOUS) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_NOEXEC) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_NOSUID) < 0) return FALSE;

    if (PyModule_AddIntMacro(module, MNT_NODEV) < 0) return FALSE;    /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_NFS4ACLS) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_UNION) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_ASYNC) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_CPROTECT) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_EXRDONLY) < 0) return FALSE; /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_EXPORTED) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_REMOVABLE) < 0) return FALSE;   /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_DEFEXPORTED) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_QUARANTINE) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_EXPORTANON) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_EXKERB) < 0) return FALSE; /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_LOCAL) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_QUOTA) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_ROOTFS) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_DOVOLFS) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_USER) < 0) return FALSE;    /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_UPDATE) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_NOBLOCK) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_DELEXPORT) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_RELOAD) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_FORCE) < 0) return FALSE;

    if (PyModule_AddIntMacro(module, MNT_DONTBROWSE) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_SUIDDIR) < 0) return FALSE;    /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_IGNORE_OWNERSHIP) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_SOFTDEP) < 0) return FALSE;          /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_AUTOMOUNTED) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_NOSYMFOLLOW) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_JOURNALED) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_IGNORE) < 0) return FALSE;    /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_NOUSERXATTR) < 0) return FALSE; /* DARWIN */
    /* FBSD: MNT_SNAPSHOT */
    if (PyModule_AddIntMacro(module, MNT_DEFWRITE) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_GJOURNAL) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_NONBUSY) < 0) return FALSE;  /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_MULTILABEL) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_NOFOLLOW) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_BYFSID) < 0) return FALSE;   /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_ACLS) < 0) return FALSE;     /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_NOATIME) < 0) return FALSE;
    if (PyModule_AddIntMacro(module, MNT_EXPUBLIC) < 0) return FALSE; /* FBSD */
    /* DARWIN: MNT_SNAPSHOT */
    if (PyModule_AddIntMacro(module, MNT_NOCLUSTERR) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_STRICTATIME) < 0) return FALSE; /* DARWIN */
    if (PyModule_AddIntMacro(module, MNT_NOCLUSTERW) < 0) return FALSE;  /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_SUJ) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_AUTOMOUNTED) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_VERIFIED) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_UNTRUSTED) < 0) return FALSE; /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_NOCOVER) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_EMPTYDIR) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_EXTLS) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_EXTLSCERT) < 0) return FALSE; /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_EXTLSCERTUSER) < 0) return FALSE; /* FBSD */

    if (PyModule_AddIntMacro(module, MNT_RECURSE) < 0) return FALSE; /* FBSD */
    if (PyModule_AddIntMacro(module, MNT_DEFERRED) < 0) return FALSE; /* FBSD */

    /* DARWIN/FBSD */
    if (PyModule_AddIntMacro(module, MNT_SNAPSHOT) < 0) return FALSE;

    return TRUE;
}

/*
 *
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"

static PyMethodDef statfs_methods[] = {
    {
        "statfs", (PyCFunction) method_statfs, METH_VARARGS | METH_KEYWORDS,
        "statfs(path: str) -> statfs\n"
    },
    {
        "fstatfs", (PyCFunction) method_fstatfs, METH_VARARGS | METH_KEYWORDS,
        "fstatfs(fd: int) -> statfs\n"
    },
    {
        "getfsstat", (PyCFunction) method_getfsstat, METH_VARARGS | METH_KEYWORDS,
        "getfsstat(flags: int = MNT_NOWAIT) -> list\n"
    },
    {
        "getmntinfo", (PyCFunction) method_getmntinfo, METH_VARARGS | METH_KEYWORDS,
        "getmntinfo(flags: int = MNT_NOWAIT) -> list\n"
    },
    {NULL, NULL, 0, NULL}, /* end */
};

#pragma GCC diagnostic pop

static PyModuleDef statfs_def = {
    PyModuleDef_HEAD_INIT,
    .m_name = "statfs",
    .m_doc = NULL,
    .m_size = -1,
    .m_methods = statfs_methods,
};

PyMODINIT_FUNC
PyInit_statfs(void)
{
    PyObject *module = NULL;

    if (!(module = PyModule_Create(&statfs_def)))
        return NULL;
    if (!prepare_module(module))
    {
        Py_DecRef(module);
        module = NULL;
    }
    return module;
}

/*
 * Local Variables:
 * c-file-style: "PEP7"
 * End:
 */
