### documentResponse.cgi
- [GET]
`curl -X GET http://localhost:8080/cgi-bin/documentResponse.cgi -v`

- [POST]
`curl -X POST http://localhost:8080/cgi-bin/documentResponse.cgi -d "name=sample" -v`


### localRedirect.cgi
`curl -X GET http://localhost:8080/cgi-bin/localRedirect.cgi -v -L`


### clientRedirectResponse.cgi
`curl -X GET http://localhost:8080/cgi-bin/clientRedirectResponse.cgi -v -L`


### clientRedirectResponseWithDocument.cgi
`curl -X GET http://localhost:8080/cgi-bin/clientRedirectResponseWithDocument.cgi -v -L`


### submit.cgi
`curl -X POST http://localhost:8080/cgi-bin/submit.cgi -d "food=sample" -v`


### infiniteLoop.cgi
`curl -X GET http://localhost:8080/cgi-bin/infiniteLoop.cgi -v`