#/bin/bash
DOCKERBIN=docker
PROJECTNAME=dlms-access-point
REPO=git@github.com:epri-dev/$PROJECTNAME.git
"$DOCKERBIN" build --no-rm --target docserver -t "beroset/$PROJECTNAME-docs" "$REPO"
"$DOCKERBIN" build --no-rm --target demo -t "beroset/$PROJECTNAME" "$REPO"
"$DOCKERBIN" build --target dashboard -t "beroset/$PROJECTNAME-dashboard" "$REPO"
