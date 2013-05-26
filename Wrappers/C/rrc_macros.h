#ifndef rrc_macrosH
#define rrc_macrosH


//=================== MACROS ================================
#define  catch_bool_macro                                   \
	catch(Exception& ex)                                    \
	{                                                       \
		stringstream msg;                                   \
		msg<<"RoadRunner exception: "<<ex.what()<<endl;  	\
		setError(msg.str());                        		\
		return false;                               		\
	}

#define  catch_ptr_macro                                    \
	catch(Exception& ex)                                    \
	{                                                       \
		stringstream msg;                                   \
		msg<<"RoadRunner exception: "<<ex.what()<<endl;  	\
		setError(msg.str());                                \
		return NULL;                                        \
	}

#define catch_int_macro                                     \
    catch(Exception& ex)                                    \
    {                                                       \
    	stringstream msg;                                   \
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;     \
        setError(msg.str());                                \
	    return -1;                                          \
    }

#endif
