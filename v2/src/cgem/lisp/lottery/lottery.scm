;;; Author: Igor Lesik 2016
;;; Copyright: Igor Lesik 2016
;;;
;;; Lottery number generator.
;;;
(use-modules (ice-9 format))
(use-modules (srfi srfi-42))

;;; randomize random seed
(set! *random-state* (random-state-from-platform))

;;; lottery range
(define lottery-range 10)

;;; print program banner
(format #t "Lottery range is ~d" lottery-range)
(newline)

;;; function to roll dice with N sides once
(define (roll-dice sides)
    (random sides) ;;; return random number in [0,sides)
)

;;; make ordered list of all lottery numbers
(define all-numbers
    (list-ec (:range n 0 lottery-range)
        n
    )
)

;;; return list where nth element moved back
(define (list-mv-back lst n)
    (append (list-head lst n)
            (list-tail lst (+ n 1))
            (list (list-ref lst n))
    )
)

(define (play iter numbers)
    (display numbers)
    (cond(
        (= iter 0)
            numbers)
        (else
            (play (- iter 1) 
                  (list-mv-back numbers (roll-dice iter))
            )
    ))
)

(define lottery-numbers
    (play lottery-range all-numbers)
)


(display lottery-numbers)
(newline)

;;; display column of lottery numbers
(do ((index 1 (+ 1 index)))   ;;; counter
    ((> index lottery-range)) ;;; until
    (format #t "~d: ~d\n"
        index (list-ref lottery-numbers (- index 1))
    )
)

