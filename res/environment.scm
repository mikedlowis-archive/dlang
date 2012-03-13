;------------------------------------------------------------------------------
; Built-in Operators
;------------------------------------------------------------------------------
(define (NE a b) (not (equal? a b)))
(define (ARRY_IDX coll idx)
  (cond
    ((list? coll)
      (list-ref coll idx))
    ((vector? coll)
      (vector-ref coll idx))
    ((hash-table? coll)
      (hash-table-ref coll idx))
    ((string? coll)
      (string-ref coll idx))))

;------------------------------------------------------------------------------
; Built-in datatype constructors
;------------------------------------------------------------------------------
(define (MAP . args) (alist->hash-table args))

;------------------------------------------------------------------------------
; Import necessary libs
;------------------------------------------------------------------------------
(require-extension srfi-1)     ; List library
(require-extension srfi-13)    ; String library
(require-extension srfi-69)    ; Hash library
(require-extension vector-lib) ; Vector library

;------------------------------------------------------------------------------
; Built-in Symbols
;------------------------------------------------------------------------------
; Null value definition
(define dl/null '())

; Boolean Value Definitions
(define dl/true #t)
(define dl/false #f)

; Type Value Definitions
(define dl/Num 'dl/Num)
(define dl/Char 'dl/Char)
(define dl/List 'dl/List)
(define dl/Vector 'dl/Vector)
(define dl/Hash 'dl/Hash)
(define dl/String 'dl/String)
(define dl/Symbol 'dl/Symbol)
(define dl/Block 'dl/Block)

;------------------------------------------------------------------------------
; Built-in Functions
;------------------------------------------------------------------------------
; Map existing scheme functions to our namespace
(define dl/display display)
(define dl/print print)
(define dl/error error)

; Map
(define dl/list_map map)
(define dl/vector_map vector-map)
(define dl/string_map string-map)
(define dl/hash_map hash-table-map)

; for_each function definitions
(define dl/list_for_each for-each)
(define dl/vector_for_each vector-for-each)
(define dl/string_for_each string-for-each)
(define dl/hash_for_each hash-table-for-each)

; any function definitions
(define dl/list_any any)
(define dl/vector_any vector-any)
(define dl/string_any string-any)

; every function definitions
(define dl/list_every every)
(define dl/vector_every vector-every)
(define dl/string_every string-every)

; fold function definitions
(define dl/list_fold fold)
(define dl/vector_fold vector-fold)
(define dl/string_fold string-fold)
(define dl/hash_table_fold hash-table-fold)

; fold_right function definitions
(define dl/list_fold_right fold-right)
(define dl/vector_fold_right vector-fold-right)
(define dl/string_fold_right string-fold-right)

; append function definitions
(define dl/list_append append)
(define dl/vector_append vector-append)
(define dl/string_append string-append)

; concat function definitions
(define dl/list_concat concatenate)
(define dl/vector_concat vector-concatenate)
(define dl/string_concat string-concatenate)
(define dl/hash_concat hash-table-merge)

; Define type symbols
(define (dl/typeof var)
  (cond
    ((number? var)     dl/Num)
    ((char? var)       dl/Char)
    ((list? var)       dl/List)
    ((vector? var)     dl/Vector)
    ((hash-table? var) dl/Hash)
    ((string? var)     dl/String)
    ((symbol? var)     dl/Symbol)
    ((procedure? var)  dl/Block) ))

(define (dl/assert cnd . msg)
  ; If condition is false
  (if (not cnd)
    ; If message is defined
    (if (not (null? msg))
      ; Display the message
      (error (car msg))
      ; Otherwise display a default message
      (error "Assertion failed.") )))

(define (dl/exec_if a b . c)
  (if a
    ; call b as function
    (b)
    ; If c exists
    (if (not (null? c))
      ; Call the head of c as a function
      ((car c)) )))

(define (dl/map . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply map args))
        ((vector? ls) (apply vector-map args))
        ((hash-table? ls) (apply hash-table-map args))
        ((string? ls) (apply string-map args)) )))

(define (dl/for_each . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply for-each args))
        ((vector? ls) (apply vector-for-each args))
        ((hash-table? ls) (apply hash-table-for-each args))
        ((string? ls) (apply string-for-each args)) )))

(define (dl/any . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply any args))
        ((vector? ls) (apply vector-any args))
        ((string? ls) (apply string-any args)) )))

(define (dl/for_all . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply every args))
        ((vector? ls) (apply vector-every args))
        ((string? ls) (apply string-every args)) )))

(define (dl/fold_left . args)
  (let ((ls (list-ref args 2)))
    (cond
        ((list? ls)   (apply fold args))
        ((vector? ls) (apply vector-fold args))
        ((hash-table? ls) (apply hash-table-fold args))
        ((string? ls) (apply string-fold args)) )))

(define (dl/fold_right . args)
  (let ((ls (list-ref args 2)))
    (cond
        ((list? ls)   (apply fold-right args))
        ((vector? ls) (apply vector-fold-right args))
        ((string? ls) (apply string-fold-right args)) )))

(define (dl/append . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply append args))
        ((vector? ls) (apply vector-append args))
        ((string? ls) (apply string-append args)) )))

(define (dl/concat . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply concatenate args))
        ((vector? ls) (apply vector-concatenate args))
        ((hash-table? ls) (apply hash-table-concatenate args))
        ((string? ls) (apply string-concatenate args)) )))

(define (error reason . args)
      (display "Error: ")
      (display reason)
      (for-each (lambda (arg)
                  (display " ")
          (write arg))
        args)
      (newline))

(define dl/error error)

(define dl/make_promise
  (lambda (proc)
    (let ((result-ready? #f)
          (result #f))
      (lambda ()
        (if result-ready?
            result
            (let ((x (proc)))
              (if result-ready?
                  result
                  (begin (set! result-ready? #t)
                         (set! result x)
                         result))))))))

(define dl/force
  (lambda (object)
    (object)))

;------------------------------------------------------------------------------
; Start User Defined Code
;------------------------------------------------------------------------------

; Potential implementation of prototype objects
;(define Proto
;  (list (make-hash-table)))
;
;(define (proto-clone p)
;  (list (make-hash-table) p))
;
;(define (proto? p))
;(define (proto-value p))
;(define (proto-member name))

