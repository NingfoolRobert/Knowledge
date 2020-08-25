#!/bin/bash
# 创建编译版本信息文件
mkdir -p generated

export LC_ALL=en_US.UTF-8
export LANG=en_US.UTF-8

PROJECT=`grep "application.*project" net/protoc/ShareProtoc/rpcalls/rpcalls.xml | sed  "s/.*project=\"\(.*\)\".*/\1/"`
IWEBVERSION=`grep "iwebversion.*default" net/scripts/iweb.xml | sed  "s/.*default=\"\([^\"]*\)\".*/\1/"`

is_git_repo=$(git rev-parse --is-inside-work-tree 2>/dev/null)
if [ "$is_git_repo" ]; then
	CODE_REPO=`git config --get remote.origin.url`
	CODE_BRANCH=`git branch | grep "^\*" | awk '{print $2}'`
	COMMIT_ID=`git rev-parse HEAD`
	COMMIT_TIME=`git log -n 1 --format="%ad" --date=local HEAD`
	XMLVERSION=`git log -1 --oneline --pretty="%h" net/rpcalls.xml`
	SVN_VERSION=""
else
	CODE_REPO=`svn info | grep "^URL:" | awk -F ": " '{print $2}'`
	CODE_BRANCH=`svn info | grep 'Relative URL' | awk -F ": " '{print $2}' | sed -e "s/\^\///"`
	COMMIT_ID=`svn info | grep "Last Changed Rev" | awk -F: '{print int($2)}'`
	COMMIT_TIME=`svn info | grep "Last Changed Date" | awk -F ": " '{print $2}' | awk '{print $1 " " $2}'`
	XMLVERSION=`svn info net/protoc/ShareProtoc/rpcalls/rpcalls.xml | grep "Last Changed Rev" | awk -F: '{print int($2)}'`
	SVN_VERSION=`svnversion -n -c | awk -F: '{print $2}'`
fi
BUILDER=`whoami`
HOSTNAME=`hostname`

cat > generated/build_info.h.temp <<EOF
#ifndef _GENERATED_BUILD_INFO_H
#define _GENERATED_BUILD_INFO_H

#define _CODE_REPO_ "${CODE_REPO}"
#define _CODE_BRANCH_ "${CODE_BRANCH}"
#define _COMMIT_ID_ "${COMMIT_ID}"
#define _COMMIT_TIME_ "${COMMIT_TIME}"
#define _XO_BUILDER_ "${BUILDER}@${HOSTNAME}"
#define XMLVERSION "$PROJECT $XMLVERSION"
#define IWEBVERSION $IWEBVERSION
#define _SVN_VERSION_ "${SVN_VERSION}"
#define PROJECT_URL "$CODE_REPO"

#endif // _GENERATED_BUILD_INFO_H
EOF

rsync -cq generated/build_info.h.temp generated/build_info.h
rm generated/build_info.h.temp
