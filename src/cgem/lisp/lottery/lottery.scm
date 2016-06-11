;;; Author: Igor Lesik 2016
;;; Copyright: Igor Lesik 2016
;;;
;;; Lottery number generator.
;;;
(use-modules (ice-9 format))

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

;;; roll dice several times
(do ((time 1 (+ 1 time)))    ;;; counter
    ((> time lottery-range)) ;;; until
    (format #t "~d: ~d\n" time (roll-dice 6))
)

