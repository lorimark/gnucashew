#line 2 "src/Dbo/AbstractSession.h"

#ifndef __SESSION_H___
#define __SESSION_H___

#include <Wt/Dbo/Session.h>

/*

CREATE TABLE gnclock
(
 Hostname varchar(255),
 PID int
);

CREATE TABLE versions
(
 table_name text(50) PRIMARY KEY NOT NULL,
 table_version integer NOT NULL
);

CREATE TABLE books
(
  guid text(32) PRIMARY KEY NOT NULL,
  root_account_guid text(32) NOT NULL,
  root_template_guid text(32) NOT NULL
);

CREATE TABLE commodities
(
 guid text(32) PRIMARY KEY NOT NULL,
 namespace text(2048) NOT NULL,
 mnemonic text(2048) NOT NULL,
 fullname text(2048),
 cusip text(2048),
 fraction integer NOT NULL,
 quote_flag integer NOT NULL,
 quote_source text(2048),
 quote_tz text(2048)
);


CREATE TABLE budgets
(
 guid text(32) PRIMARY KEY NOT NULL,
 name text(2048) NOT NULL,
 description text(2048),
 num_periods integer NOT NULL
);

CREATE TABLE budget_amounts
(
 id integer PRIMARY KEY AUTOINCREMENT NOT NULL,
 budget_guid text(32) NOT NULL,
 account_guid text(32) NOT NULL,
 period_num integer NOT NULL,
 amount_num bigint NOT NULL,
 amount_denom bigint NOT NULL
);

CREATE TABLE sqlite_sequence
(
 name,
 seq
);

CREATE TABLE prices
(
 guid text(32) PRIMARY KEY NOT NULL,
 commodity_guid text(32) NOT NULL,
 currency_guid text(32) NOT NULL,
 date text(19) NOT NULL,
 source text(2048),
 type text(2048),
 value_num bigint NOT NULL,
 value_denom bigint NOT NULL
);

CREATE TABLE slots
(
 id integer PRIMARY KEY AUTOINCREMENT NOT NULL,
 obj_guid text(32) NOT NULL,
 name text(4096) NOT NULL,
 slot_type integer NOT NULL,
 int64_val bigint,
 string_val text(4096),
 double_val float8,
 timespec_val text(19),
 guid_val text(32),
 numeric_val_num bigint,
 numeric_val_denom bigint,
 gdate_val text(8)
);
CREATE INDEX slots_guid_index ON slots(obj_guid);

CREATE TABLE recurrences
(
 id integer PRIMARY KEY AUTOINCREMENT NOT NULL,
 obj_guid text(32) NOT NULL,
 recurrence_mult integer NOT NULL,
 recurrence_period_type text(2048) NOT NULL,
 recurrence_period_start text(8) NOT NULL,
 recurrence_weekend_adjust text(2048) NOT NULL
);

CREATE TABLE schedxactions
(
 guid text(32) PRIMARY KEY NOT NULL,
 name text(2048),
 enabled integer NOT NULL,
 start_date text(8),
 end_date text(8),
 last_occur text(8),
 num_occur integer NOT NULL,
 rem_occur integer NOT NULL,
 auto_create integer NOT NULL,
 auto_notify integer NOT NULL,
 adv_creation integer NOT NULL,
 adv_notify integer NOT NULL,
 instance_count integer NOT NULL,
 template_act_guid text(32) NOT NULL
);

CREATE TABLE lots
(
 guid text(32) PRIMARY KEY NOT NULL,
 account_guid text(32),
 is_closed integer NOT NULL
);

CREATE TABLE billterms
(
 guid text(32) PRIMARY KEY NOT NULL,
 name text(2048) NOT NULL,
 description text(2048) NOT NULL,
 refcount integer NOT NULL,
 invisible integer NOT NULL,
 parent text(32),
 type text(2048) NOT NULL,
 duedays integer,
 discountdays integer,
 discount_num bigint,
 discount_denom bigint,
 cutoff integer
);

CREATE TABLE employees
(
 guid text(32) PRIMARY KEY NOT NULL,
 username text(2048) NOT NULL,
 id text(2048) NOT NULL,
 language text(2048) NOT NULL,
 acl text(2048) NOT NULL,
 active integer NOT NULL,
 currency text(32) NOT NULL,
 ccard_guid text(32),
 workday_num bigint NOT NULL,
 workday_denom bigint NOT NULL,
 rate_num bigint NOT NULL,
 rate_denom bigint NOT NULL,
 addr_name text(1024),
 addr_addr1 text(1024),
 addr_addr2 text(1024),
 addr_addr3 text(1024),
 addr_addr4 text(1024),
 addr_phone text(128),
 addr_fax text(128),
 addr_email text(256)
);

CREATE TABLE entries
(
 guid text(32) PRIMARY KEY NOT NULL,
 date text(19) NOT NULL,
 date_entered text(19),
 description text(2048),
 action text(2048),
 notes text(2048),
 quantity_num bigint,
 quantity_denom bigint,
 i_acct text(32),
 i_price_num bigint,
 i_price_denom bigint,
 i_discount_num bigint,
 i_discount_denom bigint,
 invoice text(32),
 i_disc_type text(2048),
 i_disc_how text(2048),
 i_taxable integer,
 i_taxincluded integer,
 i_taxtable text(32),
 b_acct text(32),
 b_price_num bigint,
 b_price_denom bigint,
 bill text(32),
 b_taxable integer,
 b_taxincluded integer,
 b_taxtable text(32),
 b_paytype integer,
 billable integer,
 billto_type integer,
 billto_guid text(32),
 order_guid text(32)
);

CREATE TABLE invoices
(
 guid text(32) PRIMARY KEY NOT NULL,
 id text(2048) NOT NULL,
 date_opened text(19),
 date_posted text(19),
 notes text(2048) NOT NULL,
 active integer NOT NULL,
 currency text(32) NOT NULL,
 owner_type integer,
 owner_guid text(32),
 terms text(32),
 billing_id text(2048),
 post_txn text(32),
 post_lot text(32),
 post_acc text(32),
 billto_type integer,
 billto_guid text(32),
 charge_amt_num bigint,
 charge_amt_denom bigint
);

CREATE TABLE jobs
(
 guid text(32) PRIMARY KEY NOT NULL,
 id text(2048) NOT NULL,
 name text(2048) NOT NULL,
 reference text(2048) NOT NULL,
 active integer NOT NULL,
 owner_type integer,
 owner_guid text(32)
);

CREATE TABLE orders
(
 guid text(32) PRIMARY KEY NOT NULL,
 id text(2048) NOT NULL,
 notes text(2048) NOT NULL,
 reference text(2048) NOT NULL,
 active integer NOT NULL,
 date_opened text(19) NOT NULL,
 date_closed text(19) NOT NULL,
 owner_type integer NOT NULL,
 owner_guid text(32) NOT NULL
);

CREATE TABLE taxtables
(
 guid text(32) PRIMARY KEY NOT NULL,
 name text(50) NOT NULL,
 refcount bigint NOT NULL,
 invisible integer NOT NULL,
 parent text(32)
);

CREATE TABLE taxtable_entries
(
 id integer PRIMARY KEY AUTOINCREMENT NOT NULL,
 taxtable text(32) NOT NULL,
 account text(32) NOT NULL,
 amount_num bigint NOT NULL,
 amount_denom bigint NOT NULL,
 type integer NOT NULL
);

CREATE TABLE vendors
(
 guid text(32) PRIMARY KEY NOT NULL,
 name text(2048) NOT NULL,
 id text(2048) NOT NULL,
 notes text(2048) NOT NULL,
 currency text(32) NOT NULL,
 active integer NOT NULL,
 tax_override integer NOT NULL,
 addr_name text(1024),
 addr_addr1 text(1024),
 addr_addr2 text(1024),
 addr_addr3 text(1024),
 addr_addr4 text(1024),
 addr_phone text(128),
 addr_fax text(128),
 addr_email text(256),
 terms text(32),
 tax_inc text(2048),
 tax_table text(32)
);

*/

namespace GCW {
  namespace Dbo {

class AbstractSession
: public Wt::Dbo::Session
{
  public:

    AbstractSession() {}

    /*!
    ** \brief Open a database
    **
    */
    bool open( const std::string & _path );

    const std::string & path() const { return m_path; }

    bool isOpen() const { return m_isOpen; }

//    virtual Wt::Dbo::Transaction startTransaction() const;

  protected:

    std::string m_path;
    bool m_isOpen = false;

  private:

}; // endclass Session

  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __SESSION_H___


