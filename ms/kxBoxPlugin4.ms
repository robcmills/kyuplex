plugin geometry kxBox
name:"kxBox"
category:"kyuplex"
classID:#(0x133067, 0x54374)
extends:Box
replaceUI:true
(
    parameters main rollout: kxParams
    (
        length type:#worldUnits ui:length default:1.6
        width type:#worldUnits ui:width default:1.6
        height type:#worldUnits ui:height default:1.6
        isCollisionObject type:#boolean ui:isCollisionObject default:false
        isRigidBody type:#boolean ui:isRigidBody default:false
    )
    rollout kxParams "kxParameters"
    (
        spinner length "Length" type:#worldunits range:[-100,100,0]
        spinner width "Width" type:#worldunits range:[-100,100,0]
        spinner height "Height" type:#worldunits range:[-100,100,0]
        checkbox isCollisionObject "isCollisionObject" type:#boolean
        checkbox isRigidBody "isRigidBody" type:#boolean

        on length changed val do delegate.length = val
        on width changed val do delegate.width = val
        on height changed val do ( 
            delegate.height = val
            CenterPivot $
        )
    )

    parameters colors rollout: kxColors
    (
        edge type:#color ui:edge default:black
        diffuse type:#color ui:diffuse default:gray
    )
    rollout kxColors "kxColors"
    (
        colorpicker edge "edges" type:#color 
        on edge changed val do $.wirecolor = val
        colorpicker diffuse "diffuse" type:#color 
        on diffuse changed val do $.material.diffuse = val
    )

    tool create (
        on mousePoint click do (
            delegate.width = 1.6
            delegate.length = 1.6
            delegate.height = 1.6 
            nodeTM.translation = gridPoint
            CenterPivot $
			$.wirecolor = $.edge as color
			$.material = standard()
			$.material.diffuse = $.diffuse
            #stop
        )
    )
)

