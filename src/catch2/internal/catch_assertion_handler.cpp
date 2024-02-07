
//              Copyright Catch2 Authors
// Distributed under the Boost Software License, Version 1.0.
//   (See accompanying file LICENSE.txt or copy at
//        https://www.boost.org/LICENSE_1_0.txt)

// SPDX-License-Identifier: BSL-1.0
#include <catch2/internal/catch_assertion_handler.hpp>
#include <catch2/interfaces/catch_interfaces_config.hpp>
#include <catch2/internal/catch_context.hpp>
#include <catch2/internal/catch_debugger.hpp>
#include <catch2/internal/catch_test_failure_exception.hpp>
#include <catch2/internal/catch_stringref.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <catch2/interfaces/catch_interfaces_capture.hpp>

#include <string>

namespace Catch {

    AssertionHandler::AssertionHandler
        (   StringRefBase macroName,
            SourceLineInfo const& lineInfo,
            StringRefBase capturedExpression,
            ResultDisposition::Flags resultDisposition )
    :   m_assertionInfo{ macroName, lineInfo, capturedExpression, resultDisposition },
        m_resultCapture( getResultCapture() )
    {
        m_resultCapture.notifyAssertionStarted( m_assertionInfo );
    }

    AssertionHandler::~AssertionHandler() {
        if ( !m_completed ) {
            m_resultCapture.handleIncomplete( m_assertionInfo );
        }
    }

    void AssertionHandler::handleExpr( IPrintableExpression const& expr ) {
        auto* t = expr.asTransient(); // TODO make nicer
        if (t) handleExpr(*t);
    }
    void AssertionHandler::handleExpr( ITransientExpression const& expr ) {
        m_resultCapture.handleExpr( m_assertionInfo, expr, m_reaction );
    }
    void AssertionHandler::handleMessage(ResultWas::OfType resultType, StringRefBase message) {
        m_resultCapture.handleMessage( m_assertionInfo, resultType, message, m_reaction );
    }

    auto AssertionHandler::allowThrows() const -> bool {
        return getCurrentContext().getConfig()->allowThrows();
    }

    void AssertionHandler::complete() {
        m_completed = true;
        if( m_reaction.shouldDebugBreak ) {

            // If you find your debugger stopping you here then go one level up on the
            // call-stack for the code that caused it (typically a failed assertion)

            // (To go back to the test and change execution, jump over the throw, next)
            CATCH_BREAK_INTO_DEBUGGER();
        }
        if (m_reaction.shouldThrow) {
            throw_test_failure_exception();
        }
        if ( m_reaction.shouldSkip ) {
            throw_test_skip_exception();
        }
    }

    void AssertionHandler::handleUnexpectedInflightException() {
        m_resultCapture.handleUnexpectedInflightException( m_assertionInfo, Catch::translateActiveException(), m_reaction );
    }

    void AssertionHandler::handleExceptionThrownAsExpected() {
        m_resultCapture.handleNonExpr(m_assertionInfo, ResultWas::Ok, m_reaction);
    }
    void AssertionHandler::handleExceptionNotThrownAsExpected() {
        m_resultCapture.handleNonExpr(m_assertionInfo, ResultWas::Ok, m_reaction);
    }

    void AssertionHandler::handleUnexpectedExceptionNotThrown() {
        m_resultCapture.handleUnexpectedExceptionNotThrown( m_assertionInfo, m_reaction );
    }

    void AssertionHandler::handleThrowingCallSkipped() {
        m_resultCapture.handleNonExpr(m_assertionInfo, ResultWas::Ok, m_reaction);
    }

    // This is the overload that takes a string and infers the Equals matcher from it
    // The more general overload, that takes any string matcher, is in catch_capture_matchers.cpp
    void handleExceptionMatchExpr( AssertionHandler& handler, StringRefBase str ) {
        handleExceptionMatchExpr( handler, Matchers::Equals( std::string(StringRef(str)) ) );
    }

} // namespace Catch
