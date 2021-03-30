# -42_webserv


1. Download/Clone this repo

        main
        {
            socket()
            struct serv_addr avec inet_addr()
            bind()
    
            listen()

            while select()
            {
                struct client_addr = accept()

                ( objet path = partie_config(client_addr->requete_info)

                char* result_requete = partie_search_and_http_headers(objet path, client_addr->requete_obj) ) >>>>> skip et juste printf pour l'instant

                send(result_requete)
            }
        }


+ a voir comment marche cgi