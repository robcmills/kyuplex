
--format to irrlicht xml style
--*in irrlicht y is z axis
fn vec3ToStr vec = (
	ret = ( formattedprint vec.x format:".2f" ) + ", "
	ret += ( formattedprint vec.z format:".2f" ) + ", " 
	ret += ( formattedprint vec.y format:".2f" )
	ret
)
--color as point4 to irr::video::SColorf attrib string
fn colorToStr clr = (
	ret = ( formattedprint clr.x format:".2f" ) + ", "
	ret += ( formattedprint clr.y format:".2f" ) + ", " 
	ret += ( formattedprint clr.z format:".2f" ) + ", "
	ret += ( formattedprint clr.w format:".2f" )
	ret
)
--box node dimensions to irr::core::vector3df
fn sizeToStr node = (
	ret = ( formattedprint node.width format:".2f" ) + ", "
	ret += ( formattedprint node.height format:".2f" ) + ", " 
	ret += ( formattedprint node.length format:".2f" )
	ret
)

--ensure progressGroups array is ordered properly
fn compareFn v1 v2 =
(
    local result = v1.name > v2.name
    case of (
        ( result == true ): 1
        ( result == false ): -1
        default: 0 )
)

fn exportEntities = (
	clearListener()
	
    entityDir = "C:/Users/oringe/Documents/kyuplex/kxRelease/entities/"
	local lessonNum = 1

    --parse lessonNum from fileName
    fsa = filterString maxFileName "_"
    lessonNum = fsa[2] as integer
	
	progressGroups = $*progress* as array
    qsort progressGroups compareFn
    --for p in progressGroups do print p.name

	for p = 1 to progressGroups.count do
	(
        filename = entityDir + (( lessonNum * 100 + p ) as string ) + ".xml"
		
		xDoc = XMLDocument()						
		xDoc.file = filename
		xDoc.loaded = True								
		rtNode = xDoc.getNewNode "kxRootNode"    --Create a new XMLNode THIS MUST BE CALLED!
		xDoc.AppendNode rtNode						
		rtNode = xDoc.getRootNode()				
		xDoc.SaveXML()						
		
		nodes = progressGroups[p].children 

        --sort children by name
        names_array = for n in nodes collect n.name
        sort names_array
        d = dummy()
        for i = 1 to names_array.count by 1 do (
            node_by_name = getNodeByName names_array[i]
            node_by_name.parent = d
            node_by_name.parent = progressGroups[p] 
        )
        delete d

		for i = 1 to nodes.count do 
		(
            if ( matchPattern nodes[i].name pattern:"*kxCollectable*" ) then 
            (
                if( classof nodes[i] != Box ) then continue

                cNode = xDoc.getNewNode "kxCollectable"
                rtNode.AppendNode cNode

                cAttrib = xDoc.getNewNode "attributes"
                cNode.AppendNode cAttrib

                nameNode = xDoc.getNewNode "string"
                nameNode.AddAttribute "name" "name"
                nameNode.AddAttribute "value" nodes[i].name 
                cAttrib.AppendNode nameNode

                posNode = xDoc.getNewNode "vector3d"
                posNode.AddAttribute "name" "position"
                posNode.AddAttribute "value" ( vec3ToStr nodes[i].pos )
                cAttrib.AppendNode posNode
                
                rotNode = xDoc.getNewNode "vector3d"
                rotNode.AddAttribute "name" "rotation"
                rotNode.AddAttribute "value" ( vec3ToStr ( nodes[i].rotation as eulerAngles ))
                cAttrib.AppendNode rotNode
            )
            
			else if( matchPattern nodes[i].name pattern:"*kxSpline*" ) then 
            (
                if( classof nodes[i] != SplineShape ) and 
                    ( classof nodes[i] != Line ) then continue

                splineNode = xDoc.getNewNode "kxSpline"
                rtNode.AppendNode splineNode

                splAttrib = xDoc.getNewNode "attributes"
                splineNode.AppendNode splAttrib

                nameNode = xDoc.getNewNode "string"
                nameNode.AddAttribute "name" "name"
                nameNode.AddAttribute "value" nodes[i].name 
                splAttrib.AppendNode nameNode
			
                openNode = xDoc.getNewNode "bool"
                openNode.AddAttribute "name" "isOpen"
                if( isClosed nodes[i] 1 ) then
                --if( matchPattern nodes[i].name pattern:"*Open*" ) then
                    ( openNode.AddAttribute "value" "false" ) else
                    ( openNode.AddAttribute "value" "true" ) 
                splAttrib.AppendNode openNode

				colorNode = xDoc.getNewNode "colorf"
                colorNode.AddAttribute "name" "wirecolor"
                colorNode.AddAttribute "value" ( colorToStr ( nodes[i].wirecolor as point4 ))
                splAttrib.AppendNode colorNode
				
				for j = 1 to numKnots nodes[i] 1 do  -- 1 param = 1st spline 
				(
					vtxNode = xDoc.getNewNode "vector3d"
					vtxNode.AddAttribute "name" "vertex"
					vtxNode.AddAttribute "value" ( vec3ToStr ( getKnotPoint nodes[i] 1 j ))
					splAttrib.AppendNode vtxNode
                )
            )

			else if ( matchPattern nodes[i].name pattern:"*kxLight*" ) then 
            (
                if( classof nodes[i] != Free_Light ) then continue

                lightNode = xDoc.getNewNode "kxLight"
                rtNode.AppendNode lightNode

                ltAttrib = xDoc.getNewNode "attributes"
                lightNode.AppendNode ltAttrib

                nameNode = xDoc.getNewNode "string"
                nameNode.AddAttribute "name" "name"
                nameNode.AddAttribute "value" nodes[i].name 
                ltAttrib.AppendNode nameNode

                posNode = xDoc.getNewNode "vector3d"
				posNode.AddAttribute "name" "position"
				posNode.AddAttribute "value" ( vec3ToStr ( nodes[i].pos ))
				ltAttrib.AppendNode posNode
			
				colorNode = xDoc.getNewNode "colorf"
                colorNode.AddAttribute "name" "color"
                colorNode.AddAttribute "value" ( colorToStr ( nodes[i].color as point4 ))
                ltAttrib.AppendNode colorNode

				radNode = xDoc.getNewNode "float"
				radNode.AddAttribute "name" "radius"
				radNode.AddAttribute "value" ( nodes[i].endFarAttenuation as string )
				ltAttrib.AppendNode radNode
            )

            else if ( matchPattern nodes[i].name pattern:"*kxBox*" ) then 
            (
                boxNode = xDoc.getNewNode "kxBox"
                rtNode.AppendNode boxNode

                boxAttrib = xDoc.getNewNode "attributes"
                boxNode.AppendNode boxAttrib

                nameNode = xDoc.getNewNode "string"
                nameNode.AddAttribute "name" "name"
                nameNode.AddAttribute "value" nodes[i].name 
                boxAttrib.AppendNode nameNode

                posNode = xDoc.getNewNode "vector3d"
                posNode.AddAttribute "name" "position"
                posNode.AddAttribute "value" ( vec3ToStr nodes[i].pos )
                boxAttrib.AppendNode posNode
                
                rotNode = xDoc.getNewNode "vector3d"
                rotNode.AddAttribute "name" "rotation"
                rotNode.AddAttribute "value" ( vec3ToStr ( nodes[i].rotation as eulerAngles ))
                boxAttrib.AppendNode rotNode

				sizeNode = xDoc.getNewNode "vector3d"
				sizeNode.AddAttribute "name" "size"
				sizeNode.AddAttribute "value" ( sizeToStr nodes[i] )
				boxAttrib.AppendNode sizeNode

				wcNode = xDoc.getNewNode "colorf"
                wcNode.AddAttribute "name" "edgeColor"
                wcNode.AddAttribute "value" ( colorToStr ( nodes[i].wirecolor as point4 ))
                boxAttrib.AppendNode wcNode

                dcNode = xDoc.getNewNode "colorf"
                dcNode.AddAttribute "name" "diffuse"
                dcNode.AddAttribute "value" ( colorToStr ( nodes[i].diffuse as point4 ))
                boxAttrib.AppendNode dcNode
			
				coNode = xDoc.getNewNode "boolean"
				coNode.AddAttribute "name" "isCollisionObject"
				coNode.AddAttribute "value" ( nodes[i].isCollisionObject as string )
				boxAttrib.AppendNode coNode

				rbNode = xDoc.getNewNode "boolean"
				rbNode.AddAttribute "name" "isRigidBody"
				rbNode.AddAttribute "value" ( nodes[i].isRigidBody as string )
				boxAttrib.AppendNode rbNode
            )

            else if ( matchPattern nodes[i].name pattern:"*kxCollisionCube*" ) then 
            (
                collisionNode = xDoc.getNewNode "kxCollisionCube"
                rtNode.AppendNode collisionNode

                clsnAttrib = xDoc.getNewNode "attributes"
                collisionNode.AppendNode clsnAttrib

                posNode = xDoc.getNewNode "vector3d"
                posNode.AddAttribute "name" "position"
                posNode.AddAttribute "value" ( vec3ToStr nodes[i].pos )
                clsnAttrib.AppendNode posNode
                
                rotNode = xDoc.getNewNode "vector3d"
                rotNode.AddAttribute "name" "rotation"
                rotNode.AddAttribute "value" ( vec3ToStr ( nodes[i].rotation as eulerAngles ))
                clsnAttrib.AppendNode rotNode

				sizeNode = xDoc.getNewNode "vector3d"
				sizeNode.AddAttribute "name" "size"
				sizeNode.AddAttribute "value" ( sizeToStr nodes[i] )
				clsnAttrib.AppendNode sizeNode

                --TODO: add collision groups/masks?
            )

            else if ( matchPattern nodes[i].name pattern:"*kxMesh*" ) then 
            (
                meshNode = xDoc.getNewNode "kxMesh"
                rtNode.AppendNode meshNode

                meshAttrib = xDoc.getNewNode "attributes"
                meshNode.AppendNode meshAttrib

                nameNode = xDoc.getNewNode "string"
                nameNode.AddAttribute "name" "name"
                nameNode.AddAttribute "value" nodes[i].name 
                meshAttrib.AppendNode nameNode

                posNode = xDoc.getNewNode "vector3d"
                posNode.AddAttribute "name" "position"
                posNode.AddAttribute "value" ( vec3ToStr nodes[i].pos )
                meshAttrib.AppendNode posNode
                
                rotNode = xDoc.getNewNode "vector3d"
                rotNode.AddAttribute "name" "rotation"
                rotNode.AddAttribute "value" ( vec3ToStr ( nodes[i].rotation as eulerAngles ))
                meshAttrib.AppendNode rotNode

                pathToMesh = "C:/Users/oringe/Documents/kyuplex/kxRelease/meshes/"
                pathToMesh += nodes[i].name + ".3ds"
                max group open
                select nodes[i]
                exportFile pathToMesh #noPrompt selectedOnly:true 
                max group close

				pathNode = xDoc.getNewNode "string"
				pathNode.AddAttribute "name" "mesh"
				pathNode.AddAttribute "value" pathToMesh 
				meshAttrib.AppendNode pathNode

                --TODO: add collision groups/masks?
            )

            else if ( matchPattern nodes[i].name pattern:"*kxSkyBox*" ) then 
            (
                skyBoxNode = xDoc.getNewNode "kxSkyBox"
                rtNode.AppendNode skyBoxNode

                sbxAttrib = xDoc.getNewNode "attributes"
                skyBoxNode.AppendNode sbxAttrib

                pathToTopTex = nodes[i].material[1].diffuseMap.filename
				topNode = xDoc.getNewNode "string"
				topNode.AddAttribute "name" "top"
				topNode.AddAttribute "value" pathToTopTex 
				sbxAttrib.AppendNode topNode

                pathToBotTex = nodes[i].material[2].diffuseMap.filename
				botNode = xDoc.getNewNode "string"
				botNode.AddAttribute "name" "bottom"
				botNode.AddAttribute "value" pathToBotTex 
				sbxAttrib.AppendNode botNode

                pathToLeftTex = nodes[i].material[3].diffuseMap.filename
				leftNode = xDoc.getNewNode "string"
				leftNode.AddAttribute "name" "left"
				leftNode.AddAttribute "value" pathToLeftTex 
				sbxAttrib.AppendNode leftNode

                pathToRightTex = nodes[i].material[4].diffuseMap.filename
				rightNode = xDoc.getNewNode "string"
				rightNode.AddAttribute "name" "right"
				rightNode.AddAttribute "value" pathToRightTex 
				sbxAttrib.AppendNode rightNode

                pathToFrontTex = nodes[i].material[5].diffuseMap.filename
				frontNode = xDoc.getNewNode "string"
				frontNode.AddAttribute "name" "front"
				frontNode.AddAttribute "value" pathToFrontTex 
				sbxAttrib.AppendNode frontNode

                pathToBackTex = nodes[i].material[6].diffuseMap.filename
				backNode = xDoc.getNewNode "string"
				backNode.AddAttribute "name" "back"
				backNode.AddAttribute "value" pathToBackTex 
				sbxAttrib.AppendNode backNode
            )
		)
		xDoc.PrettySaveXML()											
	)   
) --end fn exportEntities

exportEntities()

