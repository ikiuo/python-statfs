#!/usr/bin/env python3

import os
from setuptools import setup, Extension


def getenv(name, defval=None):
    if name in os.environ:
        return os.environ[name]
    return defval


def bool_getenv(name):
    return getenv(name, '').lower() in ('ok', 'true', 'yes')


DEBUG = bool_getenv('DEBUG')

MAJOR_VERSION = 0
MINOR_VERSION = 2
DEBUG_VERSION = 0
VERSION = f'{MAJOR_VERSION}.{MINOR_VERSION}.{DEBUG_VERSION}'

DEFINE_MACROS = [
    ('MAJOR_VERSION', MAJOR_VERSION),
    ('MINOR_VERSION', MINOR_VERSION),
    ('DEBUG_VERSION', DEBUG_VERSION),
]
UNDEF_MACROS = []

EXTRA_COMPILE_ARGS = [
    '-W',
    '-Wall',
    '-Wextra',
    # '-Wno-cast-function-type-mismatch',
]

if DEBUG:
    DEFINE_MACROS.append(('DEBUG', 1))
    UNDEF_MACROS.append('NDEBUG')
    EXTRA_COMPILE_ARGS.append('-O0')

MODULE_NAME = 'statfs'
PACKAGES = None
EXT_MODULES = [Extension(
    name=MODULE_NAME,
    define_macros=DEFINE_MACROS,
    undef_macros=UNDEF_MACROS,
    extra_compile_args=EXTRA_COMPILE_ARGS,
    sources=['statfs.c'],
)]

UNAME = os.uname()
UNAME_S = UNAME.sysname
UNAME_SL = UNAME_S.lower()

with open('config.h', 'w') as fp:
    config = []
    if UNAME_SL == 'darwin':
        config += [
            '#define COMPILE_DARWIN   1',
            '#define HAVE_STATFS      1',
            '#define HAVE_FSTATFS     1',
            '#define HAVE_GETFSSTAT   1',
            '#define HAVE_GETMNTINFO  1',
            '',
            '#define MNT_NFS4ACLS            0',
            '#define MNT_EXRDONLY            0',
            '#define MNT_DEFEXPORTED         0',
            '#define MNT_EXPORTANON          0',
            '#define MNT_EXKERB              0',
            '#define MNT_USER                0',
            '#define MNT_DELEXPORT           0',
            '#define MNT_SUIDDIR             0',
            '#define MNT_SOFTDEP             0',
            '#define MNT_NOSYMFOLLOW         0',
            '#define MNT_IGNORE              0',
            '#define MNT_GJOURNAL            0',
            '#define MNT_NONBUSY             0',
            '#define MNT_BYFSID              0',
            '#define MNT_ACLS                0',
            '#define MNT_EXPUBLIC            0',
            '#define MNT_NOCLUSTERR          0',
            '#define MNT_NOCLUSTERW          0',
            '#define MNT_SUJ                 0',
            '#define MNT_AUTOMOUNTED         0',
            '#define MNT_VERIFIED            0',
            '#define MNT_UNTRUSTED           0',
            '#define MNT_NOCOVER             0',
            '#define MNT_EMPTYDIR            0',
            '#define MNT_EXTLS               0',
            '#define MNT_EXTLSCERT           0',
            '#define MNT_EXTLSCERTUSER       0',
            '#define MNT_RECURSE             0',
            '#define MNT_DEFERRED            0',
        ]
    elif UNAME_SL == 'freebsd':
        config += [
            '#define COMPILE_FREEBSD  1',
            '#define HAVE_STATFS      1',
            '#define HAVE_FSTATFS     1',
            '#define HAVE_GETFSSTAT   1',
            '#define HAVE_GETMNTINFO  1',
            '',
            '#define MNT_DWAIT               MNT_WAIT',
            '',
            '#define MNT_NODEV               0',
            '#define MNT_CPROTECT            0',
            '#define MNT_REMOVABLE           0',
            '#define MNT_QUARANTINE          0',
            '#define MNT_DOVOLFS             0',
            '#define MNT_NOBLOCK             0',
            '#define MNT_DONTBROWSE          0',
            '#define MNT_IGNORE_OWNERSHIP    0',
            '#define MNT_AUTOMOUNTED         0',
            '#define MNT_JOURNALED           0',
            '#define MNT_NOUSERXATTR         0',
            '#define MNT_DEFWRITE            0',
            '#define MNT_NOFOLLOW            0',
            '#define MNT_STRICTATIME         0',
        ]
    else:
        config += [
            '#define MNT_WAIT                0',
            '#define MNT_DWAIT               0',
            '#define MNT_NOWAIT              0',
            '',
            '#define MNT_RDONLY              0',
            '#define MNT_SYNCHRONOUS         0',
            '#define MNT_NOEXEC              0',
            '#define MNT_NOSUID              0',
            #
            '#define MNT_NODEV               0',
            '#define MNT_NFS4ACLS            0',
            '#define MNT_UNION               0',
            '#define MNT_ASYNC               0',
            '#define MNT_CPROTECT            0',
            '#define MNT_EXRDONLY            0',
            #
            '#define MNT_EXPORTED            0',
            '#define MNT_REMOVABLE           0',
            '#define MNT_DEFEXPORTED         0',
            '#define MNT_QUARANTINE          0',
            '#define MNT_EXPORTANON          0',
            '#define MNT_EXKERB              0',
            #
            '#define MNT_LOCAL               0',
            '#define MNT_QUOTA               0',
            '#define MNT_ROOTFS              0',
            '#define MNT_DOVOLFS             0',
            '#define MNT_USER                0',
            #
            '#define MNT_UPDATE              0',
            '#define MNT_NOBLOCK             0',
            '#define MNT_DELEXPORT           0',
            '#define MNT_RELOAD              0',
            '#define MNT_FORCE               0',
            #
            '#define MNT_DONTBROWSE          0',
            '#define MNT_SUIDDIR             0',
            '#define MNT_IGNORE_OWNERSHIP    0',
            '#define MNT_SOFTDEP             0',
            '#define MNT_AUTOMOUNTED         0',
            '#define MNT_NOSYMFOLLOW         0',
            '#define MNT_JOURNALED           0',
            '#define MNT_IGNORE              0',
            #
            '#define MNT_NOUSERXATTR         0',
            '#define MNT_DEFWRITE            0',
            '#define MNT_GJOURNAL            0',
            '#define MNT_NONBUSY             0',
            '#define MNT_MULTILABEL          0',
            '#define MNT_NOFOLLOW            0',
            '#define MNT_BYFSID              0',
            '#define MNT_ACLS                0',
            #
            '#define MNT_NOATIME             0',
            '#define MNT_EXPUBLIC            0',
            #
            '#define MNT_NOCLUSTERR          0',
            '#define MNT_STRICTATIME         0',
            '#define MNT_NOCLUSTERW          0',
            #
            '#define MNT_SUJ                 0',
            '#define MNT_AUTOMOUNTED         0',
            '#define MNT_VERIFIED            0',
            '#define MNT_UNTRUSTED           0',
            #
            '#define MNT_NOCOVER             0',
            '#define MNT_EMPTYDIR            0',
            '#define MNT_EXTLS               0',
            '#define MNT_EXTLSCERT           0',
            #
            '#define MNT_EXTLSCERTUSER       0',
            #
            '#define MNT_RECURSE             0',
            '#define MNT_DEFERRED            0',
            #
            '#define MNT_SNAPSHOT            0',
        ]
    config.append('')
    fp.write('\n'.join(config))

setup(name=MODULE_NAME, version=VERSION,
      packages=PACKAGES, ext_modules=EXT_MODULES)
