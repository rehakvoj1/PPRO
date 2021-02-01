#pragma once

#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/Transaction.h>
#include "../Session.h"


class DAO {
public:
	DAO(Session* session) : m_session(session){ }

	template <class T>
	Wt::Dbo::ptr<T> Create( std::unique_ptr<T> objPtr ) {
		Wt::Dbo::Transaction transaction{ *m_session };
		Wt::Dbo::ptr<T> obj = m_session->add( std::move( objPtr ) );

		return obj;
	}

	template <class T>
	Wt::Dbo::ptr<T> ReadOne( Wt::Dbo::dbo_default_traits::IdType id ) {
		Wt::Dbo::Transaction transaction{ *m_session };
		Wt::Dbo::ptr<T> obj = m_session->find<T>().where( "id = ?" ).bind( id );

		return obj;
	}

	template <class T>
	Wt::Dbo::collection< Wt::Dbo::ptr<T> > ReadAll() {
		Wt::Dbo::Transaction transaction{ *m_session };
		Wt::Dbo::collection< Wt::Dbo::ptr<T> > objects;
		objects = m_session->find<T>();

		return objects;
	}

	template <class T, class U>
	Wt::Dbo::collection< Wt::Dbo::ptr<T> > FindCollectionByCondition(std::string whereClause, U whereCondition ) {
		Wt::Dbo::Transaction transaction{ *m_session };
		Wt::Dbo::collection< Wt::Dbo::ptr<T> > objects;
		objects = m_session->find<T>().where( whereClause ).bind( whereCondition );

		return objects;
	}
	
	template <class T, class U>
	Wt::Dbo::ptr<T> FindOneByCondition( std::string whereClause, U whereCondition ) {
		Wt::Dbo::Transaction transaction{ *m_session };
		Wt::Dbo::ptr<T> object;
		object = m_session->find<T>().where( whereClause ).bind( whereCondition );

		return object;
	}

	template<class T>
	bool Delete(Wt::Dbo::dbo_default_traits::IdType id) {
		Wt::Dbo::Transaction transaction{ *m_session };
	
		Wt::Dbo::ptr<T> obj = m_session->find<T>().where( "id = ?" ).bind( id );
		obj.remove();
		return obj.isTransient();
	}

private:
	Session* m_session;
};

