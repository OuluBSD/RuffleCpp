// C++ translation of core/src/compatibility_rules.rs
//! Compatibility rules for URL rewriting and blocking

#ifndef RUFFLE_CORE_COMPATIBILITY_RULES_H
#define RUFFLE_CORE_COMPATIBILITY_RULES_H

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <set>

// Forward declarations
namespace ruffle {
namespace core {
    namespace backend {
        namespace navigator {
            struct ErrorResponse;
            enum class FetchReason;
        }
    }
    namespace loader {
        enum class Error;
    }
}
}

namespace ruffle {
namespace core {

using backend::navigator::ErrorResponse;
using backend::navigator::FetchReason;

/// Stage at which URL rewrite is performed
enum class UrlRewriteStage {
    /// Perform URL rewrite before sending the request.
    /// The request will be sent to a different URL.
    BeforeRequest,

    /// Perform URL rewrite after receiving the response.
    /// The response URL will be rewritten, and SWFs will see
    /// the rewritten URL.
    AfterResponse
};

/// Rule for rewriting URLs
class UrlRewriteRule {
private:
    UrlRewriteStage stage_;
    std::set<FetchReason> fetch_reasons_;
    std::string host_;
    std::string replacement_;

public:
    UrlRewriteRule() = default;

    UrlRewriteRule(
        UrlRewriteStage stage,
        std::set<FetchReason> fetch_reasons,
        std::string host,
        std::string replacement
    ) : stage_(stage),
        fetch_reasons_(std::move(fetch_reasons)),
        host_(std::move(host)),
        replacement_(std::move(replacement)) {}

    /// Create a new URL rewrite rule
    static UrlRewriteRule new_(
        UrlRewriteStage stage,
        std::set<FetchReason> fetch_reasons,
        const std::string& host,
        const std::string& replacement
    ) {
        return UrlRewriteRule(stage, fetch_reasons, host, replacement);
    }

    // Getters
    UrlRewriteStage stage() const { return stage_; }
    const std::set<FetchReason>& fetch_reasons() const { return fetch_reasons_; }
    const std::string& host() const { return host_; }
    const std::string& replacement() const { return replacement_; }
};

/// Rule for blocking URLs
class UrlBlockRule {
private:
    std::set<FetchReason> fetch_reasons_;
    std::string host_;

public:
    UrlBlockRule() = default;

    UrlBlockRule(
        std::set<FetchReason> fetch_reasons,
        std::string host
    ) : fetch_reasons_(std::move(fetch_reasons)),
        host_(std::move(host)) {}

    /// Create a new URL block rule
    static UrlBlockRule new_(
        std::set<FetchReason> fetch_reasons,
        const std::string& host
    ) {
        return UrlBlockRule(fetch_reasons, host);
    }

    // Getters
    const std::set<FetchReason>& fetch_reasons() const { return fetch_reasons_; }
    const std::string& host() const { return host_; }
};

/// A set of rules with a name
class RuleSet {
private:
    std::string name_;
    std::vector<UrlRewriteRule> domain_rewrite_rules_;
    std::vector<UrlBlockRule> domain_block_rules_;

public:
    RuleSet() = default;

    RuleSet(
        std::string name,
        std::vector<UrlRewriteRule> domain_rewrite_rules,
        std::vector<UrlBlockRule> domain_block_rules
    ) : name_(std::move(name)),
        domain_rewrite_rules_(std::move(domain_rewrite_rules)),
        domain_block_rules_(std::move(domain_block_rules)) {}

    // Getters
    const std::string& name() const { return name_; }
    const std::vector<UrlRewriteRule>& domain_rewrite_rules() const { return domain_rewrite_rules_; }
    const std::vector<UrlBlockRule>& domain_block_rules() const { return domain_block_rules_; }
};

/// Compatibility rules for URL rewriting and blocking
class CompatibilityRules {
private:
    std::vector<RuleSet> rule_sets_;

public:
    CompatibilityRules() = default;

    explicit CompatibilityRules(std::vector<RuleSet> rule_sets)
        : rule_sets_(std::move(rule_sets)) {}

    /// Create empty compatibility rules
    static CompatibilityRules empty() {
        return CompatibilityRules();
    }

    /// Default rules for general SWF compatibility.
    /// Rules that are added here must, to the best of our ability:
    /// - Only affect content that cannot run anymore, such as requiring lost assets
    /// - Not allow people to easily pirate or cheat games more than they can already
    static CompatibilityRules builtin_rules() {
        std::vector<RuleSet> rule_sets;

        // Replaces konggames.com domains with kongregate.com to fool old sitelocks that no longer work.
        rule_sets.push_back(RuleSet{
            "kongregate_sitelock",
            {
                UrlRewriteRule::new_(
                    UrlRewriteStage::AfterResponse,
                    {FetchReason::LoadSwf},
                    "*.konggames.com",
                    "chat.kongregate.com"
                )
            },
            {}
        });

        // Replaces fpdownload.adobe.com with Ruffle's CDN. fpdownload.adobe.com hosts SWZ files
        // which do not work on web due to CORS (and the reliability of fpdownload.adobe.com is
        // questionable).
        rule_sets.push_back(RuleSet{
            "fpdownload",
            {
                UrlRewriteRule::new_(
                    UrlRewriteStage::BeforeRequest,
                    {FetchReason::UrlLoader},
                    "fpdownload.adobe.com",
                    "cdn.ruffle.rs"
                )
            },
            {}
        });

        // Mochiads currently don't work and the moachiads.com domain is up for sale.
        // There are real concerns that a malicious party could buy it.
        rule_sets.push_back(RuleSet{
            "mochiads",
            {},
            {
                UrlBlockRule::new_(
                    {FetchReason::LoadSwf, FetchReason::UrlLoader, FetchReason::NavigateToUrl},
                    "*.mochiads.com"
                )
            }
        });

        return CompatibilityRules(rule_sets);
    }

    /// Block or rewrite SWF URL based on compatibility rules
    ///
    /// @param original_url The original URL
    /// @param stage The rewrite stage to apply
    /// @param fetch_reason The reason for the fetch
    /// @return Ok(Some(rewritten_url)) if rewritten, Ok(None) if unchanged, Err if blocked
    std::variant<std::optional<std::string>, ErrorResponse> block_or_rewrite_swf_url(
        const std::string& original_url,
        UrlRewriteStage stage,
        FetchReason fetch_reason
    ) const;

    /// Get the rule sets
    const std::vector<RuleSet>& rule_sets() const { return rule_sets_; }
};

/// Tests that two domains match.
///
/// Expected string may start with `*.` to allow for any further subdomains.
bool domain_matches(const std::string& expected, const std::string& actual);

// Implementation

inline std::variant<std::optional<std::string>, ErrorResponse> 
CompatibilityRules::block_or_rewrite_swf_url(
    const std::string& original_url,
    UrlRewriteStage stage,
    FetchReason fetch_reason
) const {
    // Simplified URL parsing - real implementation would use a proper URL library
    std::string url = original_url;
    std::string host;
    
    // Extract host from URL (simplified)
    size_t protocol_end = url.find("://");
    if (protocol_end != std::string::npos) {
        size_t host_start = protocol_end + 3;
        size_t host_end = url.find('/', host_start);
        if (host_end == std::string::npos) {
            host_end = url.find('?', host_start);
        }
        if (host_end == std::string::npos) {
            host_end = url.length();
        }
        host = url.substr(host_start, host_end - host_start);
    } else {
        // Relative URL without base - return Ok(None)
        return std::optional<std::string>();
    }

    bool rewritten = false;

    for (const auto& rule_set : rule_sets_) {
        for (const auto& rule : rule_set.domain_rewrite_rules()) {
            if (rule.stage() != stage || 
                rule.fetch_reasons().find(fetch_reason) == rule.fetch_reasons().end()) {
                continue;
            }

            if (domain_matches(rule.host(), host)) {
                // In real implementation, this would log: 
                // "Rewriting swf url due to compatibility ruleset '{}'"
                // and actually rewrite the URL
                
                // Simplified: just mark as rewritten
                rewritten = true;
            }
        }

        if (stage == UrlRewriteStage::BeforeRequest) {
            for (const auto& rule : rule_set.domain_block_rules()) {
                if (rule.fetch_reasons().find(fetch_reason) == rule.fetch_reasons().end()) {
                    continue;
                }

                if (domain_matches(rule.host(), host)) {
                    // In real implementation, this would log:
                    // "Blocking url due to compatibility ruleset '{}'"
                    
                    // Return error response
                    ErrorResponse error;
                    error.url = original_url;
                    // error.error = loader::Error::BlockedHost(rule.host());
                    return error;
                }
            }
        }
    }

    if (rewritten) {
        return std::optional<std::string>(url); // Would be the rewritten URL
    } else {
        return std::optional<std::string>();
    }
}

inline bool domain_matches(const std::string& expected, const std::string& actual) {
    // Split by '.'
    std::vector<std::string> expected_parts;
    std::vector<std::string> actual_parts;
    
    // Simple split implementation
    size_t start = 0;
    size_t end = expected.find('.');
    while (end != std::string::npos) {
        expected_parts.push_back(expected.substr(start, end - start));
        start = end + 1;
        end = expected.find('.', start);
    }
    expected_parts.push_back(expected.substr(start));
    
    start = 0;
    end = actual.find('.');
    while (end != std::string::npos) {
        actual_parts.push_back(actual.substr(start, end - start));
        start = end + 1;
        end = actual.find('.', start);
    }
    actual_parts.push_back(actual.substr(start));

    // Reverse iterate for domain matching (TLD first)
    auto exp_it = expected_parts.rbegin();
    auto act_it = actual_parts.rbegin();
    bool allow_subdomains = false;

    while (exp_it != expected_parts.rend()) {
        const std::string& test = *exp_it;
        
        if (test == "*" && std::next(exp_it) == expected_parts.rend()) {
            allow_subdomains = true;
            ++exp_it;
            continue;
        }

        if (act_it == actual_parts.rend()) {
            return false;
        }

        // Case-insensitive comparison
        if (test.length() != act_it->length()) {
            return false;
        }
        
        for (size_t i = 0; i < test.length(); ++i) {
            if (std::tolower(test[i]) != std::tolower((*act_it)[i])) {
                return false;
            }
        }

        ++exp_it;
        ++act_it;
    }

    return allow_subdomains || act_it == actual_parts.rend();
}

} // namespace core
} // namespace ruffle

#endif // RUFFLE_CORE_COMPATIBILITY_RULES_H
