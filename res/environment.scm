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
(define dl/Map 'dl/Map)
(define dl/String 'dl/String)
(define dl/Symbol 'dl/Symbol)
(define dl/Block 'dl/Block)

; Named characters for escape sequences
(char-name 'null (integer->char 0))
(char-name 'formfeed (integer->char 12))

;------------------------------------------------------------------------------
; Utility Functions
;------------------------------------------------------------------------------
(define (obj-set! obj idx val)
  (cond
    ((vector? obj) (vector-set! obj idx val))
    ((hash-table? obj) (hash-table-set! obj idx val))
    ((string? obj) (string-set! obj idx val)) ))

;------------------------------------------------------------------------------
; Built-in Functions
;------------------------------------------------------------------------------

; typeof - Return the type id of the given object
(define (dl/typeof var)
  (cond
    ((number? var)     dl/Num)
    ((char? var)       dl/Char)
    ((list? var)       dl/List)
    ((vector? var)     dl/Vector)
    ((hash-table? var) dl/Map)
    ((string? var)     dl/String)
    ((symbol? var)     dl/Symbol)
    ((procedure? var)  dl/Block) ))

; Map existing scheme functions to our namespace
(define dl/display display)
(define dl/print print)
(define dl/error error)

; map function definitions
(define dl/list_map map)
(define dl/vector_map vector-map)
(define dl/string_map string-map)
(define dl/hash_map hash-table-map)
(define (dl/map . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply map args))
        ((vector? ls) (apply vector-map args))
        ((hash-table? ls) (apply hash-table-map args))
        ((string? ls) (apply string-map args)) )))

; for_each function definitions
(define dl/list_for_each for-each)
(define dl/vector_for_each vector-for-each)
(define dl/string_for_each string-for-each)
(define dl/hash_for_each hash-table-for-each)
(define (dl/for_each . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply for-each args))
        ((vector? ls) (apply vector-for-each args))
        ((hash-table? ls) (apply hash-table-for-each args))
        ((string? ls) (apply string-for-each args)) )))

; any function definitions
(define dl/list_any any)
(define dl/vector_any vector-any)
(define dl/string_any string-any)
(define (dl/any . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply any args))
        ((vector? ls) (apply vector-any args))
        ((string? ls) (apply string-any args)) )))

; every function definitions
(define dl/list_every every)
(define dl/vector_every vector-every)
(define dl/string_every string-every)
(define (dl/every . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply every args))
        ((vector? ls) (apply vector-every args))
        ((string? ls) (apply string-every args)) )))

; fold function definitions
(define dl/list_fold fold)
(define dl/vector_fold vector-fold)
(define dl/string_fold string-fold)
(define dl/hash_table_fold hash-table-fold)
(define (dl/fold . args)
  (let ((ls (list-ref args 2)))
    (cond
        ((list? ls)   (apply fold args))
        ((vector? ls) (apply vector-fold args))
        ((hash-table? ls) (apply hash-table-fold args))
        ((string? ls) (apply string-fold args)) )))

; fold_right function definitions
(define dl/list_fold_right fold-right)
(define dl/vector_fold_right vector-fold-right)
(define dl/string_fold_right string-fold-right)
(define (dl/fold_right . args)
  (let ((ls (list-ref args 2)))
    (cond
        ((list? ls)   (apply fold-right args))
        ((vector? ls) (apply vector-fold-right args))
        ((string? ls) (apply string-fold-right args)) )))

; append function definitions
(define dl/list_append append)
(define dl/vector_append vector-append)
(define dl/string_append string-append)
(define (dl/append . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply append args))
        ((vector? ls) (apply vector-append args))
        ((string? ls) (apply string-append args)) )))

; concat function definitions
(define dl/list_concat concatenate)
(define dl/vector_concat vector-concatenate)
(define dl/string_concat string-concatenate)
(define dl/hash_concat hash-table-merge)
(define (dl/concat . args)
  (let ((ls (list-ref args 1)))
    (cond
        ((list? ls)   (apply concatenate args))
        ((vector? ls) (apply vector-concatenate args))
        ((hash-table? ls) (apply hash-table-concatenate args))
        ((string? ls) (apply string-concatenate args)) )))

; exec_if - Conditional execution
(define (dl/exec_if a b . c)
  (if a
    ; call b as function
    (b)
    ; If c exists
    (if (not (null? c))
      ; Call the head of c as a function
      ((car c)) )))

;(define (error reason . args)
;      (display "Error: ")
;      (display reason)
;      (for-each (lambda (arg)
;                  (display " ")
;          (write arg))
;        args)
;      (newline))
;
;(define dl/error error)

;(define dl/make_promise
;  (lambda (proc)
;    (let ((result-ready? #f)
;          (result #f))
;      (lambda ()
;        (if result-ready?
;            result
;            (let ((x (proc)))
;              (if result-ready?
;                  result
;                  (begin (set! result-ready? #t)
;                         (set! result x)
;                         result))))))))

;------------------------------------------------------------------------------
; Start User Defined Code
;------------------------------------------------------------------------------

(define
    dl/make_promise
    (lambda
      (
        dl/expr)
      (begin
        (define
          dl/result
          dl/expr)
        (lambda
          ()
          (begin
            (apply
              dl/print
              (list
                "expr: "
                dl/expr))
            (apply
              dl/print
              (list
                "result: "
                dl/result))
            dl/result)))))
