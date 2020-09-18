#/bin/bash
DOCKERBIN=docker
PROJECTNAME=dlms-access-point
REPO=git@github.com:epri-dev/$PROJECTNAME.git
"$DOCKERBIN" build --target builder -t "beroset/$PROJECTNAME-builder" "$REPO"
"$DOCKERBIN" build --target docserver -t "beroset/$PROJECTNAME-docs" "$REPO"
"$DOCKERBIN" build --target demo -t "beroset/$PROJECTNAME" "$REPO"
"$DOCKERBIN" build --target dashboard -t "beroset/$PROJECTNAME-dashboard" "$REPO"
