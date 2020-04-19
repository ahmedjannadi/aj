a = 0
#simple comment test print("hello")

function drawcube(x,y)
	print("X: "+x+" Y: "+y)
end

function update()
	a++
end

function render()
	drawcube(a,0)
end

function loop()
	update()
	render()
end

loop()

