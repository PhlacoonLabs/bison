# C++ skeleton for Bison

# Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008
# Free Software Foundation, Inc.

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

m4_include(b4_pkgdatadir/[c++.m4])

# b4_table_define(TABLE-NAME, CONTENT)
# ------------------------------------
# Define "parser::yy<TABLE-NAME>_" which contents is CONTENT.
m4_define([b4_table_define],
[const b4_int_type_for([$2])
  b4_parser_class_name::yy$1_[[]] =
  {
  $2
  }dnl
])

# How the semantic value is extracted when using variants.
b4_variant_if([
  # b4_symbol_value(VAL, [TYPE])
  # ----------------------------
  m4_define([b4_symbol_value],
  [m4_ifval([$2],
            [$1.as<$2>()],
            [$1])])
]) # b4_variant_if


# b4_assert_if([IF-ASSERTIONS-ARE-USED], [IF-NOT])
# ----------------------------------------------------
m4_define([b4_assert_if],
[b4_percent_define_ifdef([[assert]], [$1], [$2])])


# b4_rhs_value(RULE-LENGTH, NUM, [TYPE])
# --------------------------------------
# Expansion of $<TYPE>NUM, where the current rule has RULE-LENGTH
# symbols on RHS.
m4_define([b4_rhs_value],
          [b4_symbol_value([yystack_@{($1) - ($2)@}.value], [$3])])

# b4_rhs_location(RULE-LENGTH, NUM)
# ---------------------------------
# Expansion of @NUM, where the current rule has RULE-LENGTH symbols
# on RHS.
m4_define([b4_rhs_location],
[yystack_@{($1) - ($2)@}.location])


# b4_symbol_actions(FILENAME, LINENO,
#                   SYMBOL-TAG, SYMBOL-NUM,
#                   SYMBOL-ACTION, SYMBOL-TYPENAME)
# -------------------------------------------------
# Same as in C, but using references instead of pointers.
m4_define([b4_symbol_actions],
[m4_pushdef([b4_dollar_dollar],
            [b4_symbol_value([yyvalue], [$6])])dnl
m4_pushdef([b4_at_dollar], [yylocation])dnl
      case $4: // $3
b4_syncline([$2], [$1])
        $5;
b4_syncline([@oline@], [@ofile@])
        break;
m4_popdef([b4_at_dollar])dnl
m4_popdef([b4_dollar_dollar])dnl
])


# b4_symbol_action_(SYMBOL-TAG, SYMBOL-NUM, SYMBOL-TYPENAME)
# ----------------------------------------------------------
# Invoke b4_dollar_dollar(SYMBOL_TYPENAME) for each symbol.
m4_define([b4_symbol_action_],
[m4_ifval($3,
[      case $2: // $1
        b4_dollar_dollar($@);
	break;
])])


# b4_symbol_variant(YYTYPE, YYVAL, ACTION, [ARGS])
# ------------------------------------------------
# Run some ACTION ("build", or "destroy") on YYVAL of symbol type
# YYTYPE.
m4_define([b4_symbol_variant],
[m4_pushdef([b4_dollar_dollar],
            [$2.$3<$][3>(m4_shift3($@))])dnl
  switch ($1)
    {
m4_map([b4_symbol_action_], m4_defn([b4_type_names]))
     default:
       break;
    }
m4_popdef([b4_dollar_dollar])dnl
])


# _b4_char_sizeof_counter
# -----------------------
# A counter used by _b4_char_sizeof_dummy to create fresh symbols.
m4_define([_b4_char_sizeof_counter],
[0])

# _b4_char_sizeof_dummy
# ---------------------
# At each call return a new C++ identifier.
m4_define([_b4_char_sizeof_dummy],
[m4_define([_b4_char_sizeof_counter], m4_incr(_b4_char_sizeof_counter))dnl
dummy[]_b4_char_sizeof_counter])


# b4_char_sizeof(SYMBOL-TAG, SYMBOL-NUM, SYMBOL-TYPENAME)
# -------------------------------------------------------
# To be mapped on the list of type names to produce:
#
#    char dummy1[sizeof(type_name_1)];
#    char dummy2[sizeof(type_name_2)];
#
# for defined type names.
# $3 is doubly-quoted, do not quote it again.
m4_define([b4_char_sizeof],
[m4_ifval($3,
[
      char _b4_char_sizeof_dummy@{sizeof($3)@}; // $1])dnl
])


m4_pushdef([b4_copyright_years],
           [2002, 2003, 2004, 2005, 2006, 2007, 2008])

m4_define([b4_parser_class_name],
          [b4_percent_define_get([[parser_class_name]])])

# The header is mandatory.
b4_defines_if([],
              [b4_fatal([b4_skeleton[: using %%defines is mandatory]])])

# Backward compatibility.
m4_define([b4_location_constructors])
m4_include(b4_pkgdatadir/[location.cc])

# We do want M4 expansion after # for CPP macros.
m4_changecom()
m4_divert_push(0)dnl
b4_defines_if(
[@output(b4_spec_defines_file@)@
b4_copyright([Skeleton interface for Bison LALR(1) parsers in C++])
dnl FIXME: This is wrong, we want computed header guards.
[
/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
# define PARSER_HEADER_H

]b4_percent_code_get([[requires]])[

]b4_assert_if([#include <cassert>])[
#include <string>
#include <iostream>
#include "stack.hh"

]b4_namespace_open[
  class position;
  class location;
]b4_variant_if(
[[
  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {]b4_assert_if([
    /// Whether something is contained.
    bool built;

    /// Initially uninitialized.
    variant ()
      : built(false)
    {}])[

    /// Instantiate a \a T in here.
    template <typename T>
    inline T&
    build()
    {]b4_assert_if([
      assert(!built);
      built = true;])[
      return *new (buffer) T;
    }

    /// Accessor to a built \a T.
    template <typename T>
    inline T&
    as()
    {]b4_assert_if([
      assert(built);])[
      return reinterpret_cast<T&>(buffer);
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    inline const T&
    as() const
    {]b4_assert_if([
      assert(built);])[
      return reinterpret_cast<const T&>(buffer);
    }

    /// Swap the content with \a other.
    template <typename T>
    inline void
    swap(variant<S>& other)
    {
      std::swap(as<T>(), other.as<T>());
    }

    /// Assign the content of \a other to this.
    /// Destroys \a other.
    template <typename T>
    inline void
    build(variant<S>& other)
    {
      build<T>();
      swap<T>(other);
      other.destroy<T>();
    }

    /// Destroy the stored \a T.
    template <typename T>
    inline void
    destroy()
    {
      as<T>().~T();]b4_assert_if([
      built = false;])[
    }

    /// A buffer large enough to store any of the semantic values.
    char buffer[S];
  };
]])[
]b4_namespace_close[

#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG ]b4_debug_flag[
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE ]b4_error_verbose_flag[
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE ]b4_token_table[
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                        \
do {                                                            \
  if (N)                                                        \
    {                                                           \
      (Current).begin = (Rhs)[1].location.begin;                \
      (Current).end   = (Rhs)[N].location.end;			\
    }                                                           \
  else                                                          \
    {                                                           \
      (Current).begin = (Current).end = (Rhs)[0].location.end;	\
    }                                                           \
} while (false)
#endif

]b4_namespace_open[

  /// A Bison parser.
  class ]b4_parser_class_name[
  {
  public:
#ifndef YYSTYPE
]b4_variant_if(
[    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {]m4_map([b4_char_sizeof], m4_defn([b4_type_names]))[
    };

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;],
[    /// Symbol semantic values.
m4_ifdef([b4_stype],
[    union semantic_type
    {b4_user_stype
    };],
[m4_if(b4_tag_seen_flag, 0,
[[    typedef int semantic_type;]],
[[    typedef YYSTYPE semantic_type;]])])])[
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef ]b4_percent_define_get([[location_type]])[ location_type;
    /// Tokens.
    struct token
    {
      ]b4_token_enums(b4_tokens)[
    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    ]b4_parser_class_name[ (]b4_parse_param_decl[);
    virtual ~]b4_parser_class_name[ ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvalue      Its semantic value.
    /// \param yylocation   Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type& yyvalue,
					 const location_type& yylocation);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvalue      Its semantic value.
    /// \param yylocation   Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type& yyvalue,
				   const location_type& yylocation);
#endif

    /// State numbers.
    typedef int state_type;

    /// Internal symbol numbers.
    typedef ]b4_int_type_for([b4_translate])[ token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const ]b4_int_type_for([b4_pact])[ yypact_[];
    static const ]b4_int_type(b4_pact_ninf, b4_pact_ninf)[ yypact_ninf_;

    /// For a state, default rule to reduce.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const ]b4_int_type_for([b4_defact])[ yydefact_[];

    static const ]b4_int_type_for([b4_pgoto])[ yypgoto_[];
    static const ]b4_int_type_for([b4_defgoto])[ yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const ]b4_int_type_for([b4_table])[ yytable_[];
    static const ]b4_int_type(b4_table_ninf, b4_table_ninf)[ yytable_ninf_;

    static const ]b4_int_type_for([b4_check])[ yycheck_[];

    /// For a state, its accessing symbol.
    static const ]b4_int_type_for([b4_stos])[ yystos_[];

    /// For a rule, its LHS.
    static const ]b4_int_type_for([b4_r1])[ yyr1_[];
    /// For a rule, its RHS length.
    static const ]b4_int_type_for([b4_r2])[ yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    /// A `-1'-separated list of the rules' RHS.
    static const ]b4_int_type_for([b4_rhs])[ yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const ]b4_int_type_for([b4_prhs])[ yyprhs_[];
    /// For each rule, its source line number.
    static const ]b4_int_type_for([b4_rline])[ yyrline_[];
    /// For each scanner token number, its symbol number.
    static const ]b4_int_type_for([b4_toknum])[ yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    ///                     If null, nothing is printed at all.
    /// \param yytype       The symbol type.
    /// \param yyvalue      Its semantic value.
    /// \param yylocation   Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type& yyvalue,
			     location_type& yylocation);

    /// Element of the stack: a state and its attributes.
    struct data_type
    {
      /// Default constructor.
      data_type ();

      /// Constructor.
      data_type (state_type s, const semantic_type& v, const location_type& l);

      /// The state.
      state_type state;

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;
    };

    /// Stack type.
    typedef stack<data_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \warning the contents of \a v is stolen.
    inline void yypush_ (state_type s,
                         semantic_type& v, const location_type& l);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;
]b4_parse_param_vars[
  };
]b4_namespace_close[

]b4_percent_define_flag_if([[global_tokens_and_yystype]],
[b4_token_defines(b4_tokens)

#ifndef YYSTYPE
 /* Redirection for backward compatibility.  */
# define YYSTYPE b4_namespace_ref::b4_parser_class_name::semantic_type
#endif
])
b4_percent_code_get([[provides]])[]dnl

[#endif /* ! defined PARSER_HEADER_H */]
])dnl
@output(b4_parser_file_name@)@
b4_copyright([Skeleton implementation for Bison LALR(1) parsers in C++])
b4_percent_code_get([[top]])[]dnl
m4_if(b4_prefix, [yy], [],
[
// Take the name prefix into account.
#define yylex   b4_prefix[]lex])[

/* First part of user declarations.  */
]b4_user_pre_prologue

b4_defines_if([[
#include "@basename(]b4_spec_defines_file[@)"]])[

/* User implementation prologue.  */
]b4_user_post_prologue
b4_percent_code_get[]dnl

[#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
  do {							\
    if (yydebug_)                                       \
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
  } while (false)

# define YY_REDUCE_PRINT(Rule)		\
  do {					\
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);		\
  } while (false)

# define YY_STACK_PRINT()		\
  do {					\
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)  static_cast<void>(0)
# define YY_REDUCE_PRINT(Rule)                          static_cast<void>(0)
# define YY_STACK_PRINT()                               static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

]b4_namespace_open[
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  ]b4_parser_class_name[::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  ]b4_parser_class_name::b4_parser_class_name[ (]b4_parse_param_decl[)]m4_ifset([b4_parse_param], [
    :])[
#if YYDEBUG
    ]m4_ifset([b4_parse_param], [  ], [ :])[yydebug_ (false),
      yycdebug_ (&std::cerr)]m4_ifset([b4_parse_param], [,])[
#endif]b4_parse_param_cons[
  {
  }

  ]b4_parser_class_name::~b4_parser_class_name[ ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  ]b4_parser_class_name[::yy_symbol_value_print_ (int yytype,
			   const semantic_type& yyvalue, const location_type& yylocation)
  {
    YYUSE (yylocation);
    YYUSE (yyvalue);
    switch (yytype)
      {
  ]m4_map([b4_symbol_actions], m4_defn([b4_symbol_printers]))dnl
[       default:
	  break;
      }
  }


  void
  ]b4_parser_class_name[::yy_symbol_print_ (int yytype,
			   const semantic_type& yyvalue, const location_type& yylocation)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << yylocation << ": ";
    yy_symbol_value_print_ (yytype, yyvalue, yylocation);
    *yycdebug_ << ')';
  }
#endif

  void
  ]b4_parser_class_name[::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type& yyvalue, location_type& yylocation)
  {
    YYUSE (yymsg);
    YYUSE (yyvalue);
    YYUSE (yylocation);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvalue, yylocation);

    // User destructor.
    switch (yytype)
      {
  ]m4_map([b4_symbol_actions], m4_defn([b4_symbol_destructors]))[
	default:
	  break;
      }]b4_variant_if([

    // Type destructor.
  b4_symbol_variant([[yytype]], [[yyvalue]], [[destroy]])])[
  }

  ]b4_parser_class_name[::data_type::data_type ()
    : state()
    , value()
    , location()
  {
  }

  ]b4_parser_class_name[::data_type::data_type (state_type s,
                           const semantic_type& v, const location_type& l)
    : state(s)
    , value(v)
    , location(l)
  {
  }

  void
  ]b4_parser_class_name[::yypush_ (state_type s,
                           semantic_type& v, const location_type& l)
  {
]b4_variant_if(
[[    yystack_.push (data_type (s, semantic_type(), l));
    ]b4_symbol_variant([[yystos_[s]]], [[yystack_[0].value]], [build], [v])],
[    yystack_.push (data_type (s, v, l));])[
  }

  void
  ]b4_parser_class_name[::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  ]b4_parser_class_name[::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  ]b4_parser_class_name[::debug_level_type
  ]b4_parser_class_name[::debug_level () const
  {
    return yydebug_;
  }

  void
  ]b4_parser_class_name[::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  ]b4_parser_class_name[::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    data_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @@$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;

]m4_ifdef([b4_initial_action], [
m4_pushdef([b4_at_dollar],     [yylloc])dnl
m4_pushdef([b4_dollar_dollar], [yylval])dnl
    /* User initialization code.  */
    b4_user_initial_action
m4_popdef([b4_dollar_dollar])dnl
m4_popdef([b4_at_dollar])])dnl

  [  /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_ = stack_type (0);
    yypush_ (yystate, yylval, yylloc);

    // A new state was pushed on the stack.
    // Invariant: yystate == yystack_[0].state, i.e.,
    // yystate was just pushed onto the state stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = ]b4_c_function_call([yylex], [int],
				     [[YYSTYPE*], [&yylval]][]dnl
b4_locations_if([, [[location*], [&yylloc]]])dnl
m4_ifdef([b4_lex_param], [, ]b4_lex_param))[;
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, yylval, yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, yylval, yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    yypush_ (yystate, yylval, yylloc);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];]b4_variant_if([
    /* Variants are always initialized to an empty instance of the
       correct type. The default $$=$1 rule is NOT applied when using
       variants */
    ]b4_symbol_variant([[yyr1_@{yyn@}]], [yyval], [build])[],[
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yystack_@{yylen - 1@}.value;
    else
      yyval = yystack_@{0@}.value;])[

    // Compute the default @@$.
    {
      slice<data_type, stack_type> slice (yystack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	]b4_user_actions[
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], yyval, yyloc);
]b4_variant_if([[
    // Destroy the lhs symbols.
    for (int i = 0; i < yylen; ++i)
      // Destroy a variant which value may have be swapped with yyval.
      // The value of yyval (hence maybe one of these lhs symbols)
      // depends on what does the default contruction for this type.
      // In the case of pointers for instance, nothing is done, so the
      // value is junk.  Therefore do not try to report the content in
      // the debug trace, it's junk.  Hence yymsg = 0.  Besides, that
      // keeps exactly the same traces as with the other Bison
      // skeletons.
      yydestruct_ (0,
                   yystos_[yystack_[i].state],
                   yystack_[i].value, yystack_[i].location);]])[

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystack_[0].state;
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystack_[0].state)
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    yypush_ (yystate, yyval, yyloc);
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0].location = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
            /* Return failure if at end of input.  */
            if (yychar == yyeof_)
              YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, yylval, yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystack_[0].state;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	// Pop the current state because it cannot handle the error token.
	if (yystack_.size () == 1)
	  YYABORT;

	yyerror_range[0].location = yystack_[0].location;
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     yystack_[0].value, yystack_[0].location);
	yypop_ ();
	yystate = yystack_[0].state;
	YY_STACK_PRINT ();
      }

    yyerror_range[1].location = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     yystack_[0].value, yystack_[0].location);

    yystate = yyn;
    yypush_ (yystate, yylval, yyloc);
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, yylval, yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystack_.size () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystack_[0].state],
		   yystack_[0].value, yystack_[0].location);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  ]b4_parser_class_name[::yysyntax_error_ (int yystate, int]dnl
b4_error_verbose_if([ tok])[)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const ]b4_int_type(b4_pact_ninf, b4_pact_ninf) b4_parser_class_name::yypact_ninf_ = b4_pact_ninf[;
  ]b4_table_define([pact], [b4_pact])[;

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  ]b4_table_define([defact], [b4_defact])[;

  /* YYPGOTO[NTERM-NUM].  */
  ]b4_table_define([pgoto], [b4_pgoto])[;

  /* YYDEFGOTO[NTERM-NUM].  */
  ]b4_table_define([defgoto], [b4_defgoto])[;

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const ]b4_int_type(b4_table_ninf, b4_table_ninf) b4_parser_class_name::yytable_ninf_ = b4_table_ninf[;
  ]b4_table_define([table], [b4_table])[;

  /* YYCHECK.  */
  ]b4_table_define([check], [b4_check])[;

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  ]b4_table_define([stos], [b4_stos])[;

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  ]b4_table_define([token_number], [b4_toknum])[;
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  ]b4_table_define([r1], [b4_r1])[;

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  ]b4_table_define([r2], [b4_r2])[;

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const ]b4_parser_class_name[::yytname_[] =
  {
  ]b4_tname[
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  ]b4_table_define([rhs], [b4_rhs])[;

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  ]b4_table_define([prhs], [b4_prhs])[;

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  ]b4_table_define([rline], [b4_rline])[;

  // Print the state stack on the debug stream.
  void
  ]b4_parser_class_name[::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
	 i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  ]b4_parser_class_name[::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       ]b4_rhs_value(yynrhs, yyi + 1)[,
		       ]b4_rhs_location(yynrhs, yyi + 1)[);
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  ]b4_parser_class_name[::token_number_type
  ]b4_parser_class_name[::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
  ]b4_translate[
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int ]b4_parser_class_name[::yyeof_ = 0;
  const int ]b4_parser_class_name[::yylast_ = ]b4_last[;
  const int ]b4_parser_class_name[::yynnts_ = ]b4_nterms_number[;
  const int ]b4_parser_class_name[::yyempty_ = -2;
  const int ]b4_parser_class_name[::yyfinal_ = ]b4_final_state_number[;
  const int ]b4_parser_class_name[::yyterror_ = 1;
  const int ]b4_parser_class_name[::yyerrcode_ = 256;
  const int ]b4_parser_class_name[::yyntokens_ = ]b4_tokens_number[;

  const unsigned int ]b4_parser_class_name[::yyuser_token_number_max_ = ]b4_user_token_number_max[;
  const ]b4_parser_class_name[::token_number_type ]b4_parser_class_name[::yyundef_token_ = ]b4_undef_token_number[;

]b4_namespace_close[

]b4_epilogue[]dnl
@output(b4_dir_prefix[]stack.hh@)@
b4_copyright([Stack handling for Bison parsers in C++])[

#ifndef BISON_STACK_HH
# define BISON_STACK_HH

#include <deque>

]b4_namespace_open[
  template <class T, class S = std::deque<T> >
  class stack
  {
  public:

    // Hide our reversed order.
    typedef typename S::reverse_iterator iterator;
    typedef typename S::const_reverse_iterator const_iterator;

    stack () : seq_ ()
    {
    }

    stack (unsigned int n) : seq_ (n)
    {
    }

    inline
    T&
    operator [] (unsigned int i)
    {
      return seq_[i];
    }

    inline
    const T&
    operator [] (unsigned int i) const
    {
      return seq_[i];
    }

    inline
    void
    push (const T& t)
    {
      seq_.push_front (t);
    }

    inline
    void
    pop (unsigned int n = 1)
    {
      for (; n; --n)
	seq_.pop_front ();
    }

    inline
    typename S::size_type
    size () const
    {
      return seq_.size ();
    }

    inline const_iterator begin () const { return seq_.rbegin (); }
    inline const_iterator end ()   const { return seq_.rend ();   }

  private:
    /// The wrapped container.
    S seq_;
  };

  /// Present a slice of the top of a stack.
  template <class T, class S = stack<T> >
  class slice
  {
  public:

    slice (const S& stack,
	   unsigned int range) : stack_ (stack),
				 range_ (range)
    {
    }

    inline
    const T&
    operator [] (unsigned int i) const
    {
      return stack_[range_ - i];
    }

  private:

    const S& stack_;
    unsigned int range_;
  };
]b4_namespace_close[

#endif // not BISON_STACK_HH[]dnl
]
m4_divert_pop(0)
m4_popdef([b4_copyright_years])dnl