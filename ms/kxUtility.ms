utility kxUtility "kxDescription" -- define the utility name and description string
(
    local last_amt = 0 -- define and initialize local variable
    checkbox x "Spread in x" -- create 3 checkboxes
    checkbox y "Spread in y"
    checkbox z "Spread in z"
    spinner spread "Spread amount:" range:[-1000,1000,0] -- create a spinner
    on spread changed amt do -- when spinner value changes...
    (
        delta = amt - last_amt -- calculate difference in current and previous
        for obj in selection do -- values for each selected object
        (
            -- calculate new position based on current position and selection center
            p = obj.pos + normalize (obj.pos - selection.center) * delta
            if x.checked then obj.pos.x = p.x -- if checkbox x checked, apply X position
            if y.checked then obj.pos.y = p.y -- if checkbox y checked, apply Y position
            if z.checked then obj.pos.z = p.z -- if checkbox z checked, apply Z position
        )
        last_amt = amt -- store spinner value as previous value
    ) -- end of "on spread changed"
) -- end of utility definition

