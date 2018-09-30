class MeasureSerializer < ActiveModel::Serializer
  attributes :id,
             :value,
             :patient,
             :sensor,
             :created_at
end
