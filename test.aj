a = 0
#simple comment test print("hello")

function draw_cube(x,y)
	print("X: "+x+" Y: "+y)
end

function update()
	a++
end

function render()
	draw_cube(a,0)
end

function loop()
	update()
	render()
end

loop()

