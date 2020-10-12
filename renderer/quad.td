#lang transd

class Quad: {
	vertX: Double(), 
	vertY: Double(), 
	coA: Double(),
	init: (λ x Double() y Double() a Double()
					(set vertX x) (set vertY y) (set coA a)),
	shade: (λ x Double() y Double() hw Int() byten Int() v Vector( Int() )
					(with t 0.0 f 0.0 shade 0
						(= t (- (sqrt y) (abs (- hw x))))
										(set f 
											(if (and (<= t 1.0)(>= t -1.0))
												(- 1.0 (abs t))
												0
											)
										)
										(set shade (* 255.0 (- 1.0 f)))
										(set v byten shade)
										(set v (+ byten 1) shade)
										(set v (+ byten 2) shade)

	))
}
