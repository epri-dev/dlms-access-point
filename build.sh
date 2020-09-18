#/bin/bash
DOCKERBIN=docker
PROJECTNAME=dlms-access-point
REPO=git@github.com:epri-dev/$PROJECTNAME.git
"$DOCKERBIN" build --target builder -t "beroset/$PROJECTNAME-builder" "$REPO"
"$DOCKERBIN" build --target docserver -t "beroset/$PROJECTNAME-docs" "$REPO#:src/docker/docserver"
"$DOCKERBIN" build --target demo -t "beroset/$PROJECTNAME" "$REPO#:src/docker/demo"
"$DOCKERBIN" build --target dashboard -t "beroset/$PROJECTNAME-dashboard" "$REPO#:src/docker/dashboard"
