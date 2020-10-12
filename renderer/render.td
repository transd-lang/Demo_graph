#lang transd

private module module1: {
	import: "Export",
	import: "Quad",
	quad: Quad(0.0 0.0 1.0),
	_start: (λ 	(resize v (* (* height width) 3)) 
							(with hw (/ width 2) n 0 m 0 t 0.0 f 0.0 shade 0 k 0 byten 0
								(while (< n width) (+= n 1)
									(while (< m height) (+= m 1)
										(set k (- (- height m) 1))
										(set byten (+ (* 3 (* k width))(* 3 n)))
										(shade quad (toDouble n) (toDouble m) hw byten v)
									)
									(= m 0)
								)
								(textout "vector size: " (size v))
						 )
					)
}
