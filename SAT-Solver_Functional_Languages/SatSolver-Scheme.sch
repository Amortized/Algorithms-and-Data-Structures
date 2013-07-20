
;;;  Sat Solver Implementation - http://en.wikipedia.org/wiki/Boolean_satisfiability_problem

(define allassign 
    (lambda (n)
      (if (= n 0)
          '()
           (letrec ((iter (lambda (list1 iterNo)
            (if (= iterNo n)
                list1
                (if (null? list1) 
                     (iter (append list1 '(#t #f)) (+ iterNo 1))
                     (iter (append (map trueappender list1) (map falseappender list1) ) (+ iterNo 1)) 
                   )))))(iter (list '()) 0)))))


(define falseappender
  (lambda (x)
    (append x '(#f))
    ))

(define trueappender
  (lambda (x)
    (append x '(#t))
    ))


(define getClauses
  (lambda (data)
    (letrec ( (iter
               (lambda (list1 list2 list3)
                 (if (null? list1)
                     (reverse list2)
                     (if (= (car list1) 0)
                         (begin (set! list2 (cons (reverse list3) list2)) (iter (cdr list1) list2 '()) )
                         (begin (set! list3 (cons (car list1) list3)) (iter (cdr list1) list2 list3) )
                          
                              )
                     )
                 )
             )  
            )
      (iter data '() '())
            
      )))



(define (eval-var n 	my)
  (do (
       (i 1 (+ i 1)))
      ((= i n) (car my))
    (set! my (cdr my))))


(define (eval-lit n mylist)
  (if
   (< n 0)
   (not (eval-var (* n -1) mylist))
   (eval-var n mylist)))


(define (eval-clause clause mylist)
  (define flag 0)
  (map (lambda(n) (if (eval-lit n mylist)
                      (set! flag 1)
                      #t                         
                  )) clause)
  (if (= flag 1)
      #t
      #f
  )
)


(define (eval-form mycnf mylist)
  (define myclauses (getClauses mycnf))
  (define flag 0)
  (map (lambda(clause) (if (eval-clause clause mylist)
                       #t
                       (set! flag 1)

                                )) myclauses)
  (if (= flag 1)
      #f
      #t
      )
  )



(define (maxvar data)
 (cond
  ((null? data) 0)
  (#t (let* ((max (maxvar (cdr data)))
  (c (car data))
  (a (if (< c 0) (- c) c)))
  (if (> a max) a max)))))


(define (sat mylist)
  (let ((largestNo (maxvar mylist)))
  (define possibleValues  (allassign largestNo))
  (define flag 0)
  (map (lambda(value) (if (eval-form mylist value)
                       (set! flag 1)
                       #t
                                )) possibleValues)
  (if (= flag 1)
      #t
      #f
      )
  ))

-------------------------------

