function collision_ball_p1(ball_x, ball_y, p1_x, p1_y, p_width, p_height)
	
	a = ball_y < p1_y + p_height
	print(ball_y < p1_y + p_height)

	print("a")
	print(a)

	a = a and ball_x < p1_x + p_width
	print(ball_x < p1_y + p_width)

	print("a")
	print(a)

	a = a and ball_x > p1_x
	print(ball_x > p1_x)

	print("a")
	print(a)

	a = a and ball_y > p1_y
	print(ball_y > p1_y)

	print("a")
	print(a)

	return a
end

collision_ball_p1(400,300,10,300,10,50)
