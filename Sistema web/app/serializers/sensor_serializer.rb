class SensorSerializer < ActiveModel::Serializer
  attributes :id,
             :name,
             :reads,
             :description,
             :created_at
end
