


#Skeleton:
- Choose next pixel to generate
- Get color at coordinate
- Decide ideal pixel color
- Find ideal available color
- Set pixel
- Add coords to queue


#Plan:
write an implementation in rainbowFactory, then generalize it.

#Thoughts:
- closest available color should not necessarily be customizable, as the closest color available *should* always return the same color, regardless of implementation (except in the case that multiple colors are equally distant from the desired color). Perhaps it would make sense to rename the function "ideal available color", as that would allow different implementations to decide for themselves which colors are more ideal than others when the preferred color is not available.

#ChooseNextPixelCoord
- Really, there's not much opportunity for customization here. The customization comes more from adding the coordinates to the queue.

#GetColorPointAtCoord
- Customizing this will change how getPreferredColor interacts with space. Lots of opportunity for strange things.
- Could create color-wormholes
- Could make the edges of the screen wrap around


#Options for GetIdealAllowedColor:
- Emptying list method
	- Slow
- Octant-sphere method
- cube-to-sphere method
	- is this one possible?
	- Check out 3DVis2.scad for diagrams.
- Emptying cube sublists
	- This can work together with the other methods.
	- This is a good idea.

#Ideas for GetIdealAllowedColor:
- Weigh differences in hue differently than differences in saturation and value
- Make it search for colors that are slightly different instead of as close as possible